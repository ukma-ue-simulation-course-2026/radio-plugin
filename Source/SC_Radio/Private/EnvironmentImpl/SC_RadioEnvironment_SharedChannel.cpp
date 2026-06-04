// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvironmentImpl/SC_RadioEnvironment_SharedChannel.h"

#include "ActorComponents/SC_ReceiverComponent.h"
#include "ActorComponents/SC_TransmitterComponent.h"

float USC_RadioEnvironment_SharedChannel::DbmToMilliwatts(float Dbm)
{
	return FMath::Pow(10.0f, Dbm / 10.0f);
}

bool USC_RadioEnvironment_SharedChannel::BroadcastMessage(const FSC_RadioBroadcastParams& Params,
														  USC_RadioMessage* Message)
{
	FSC_PendingTransmission Entry;
	Entry.Params = Params;
	Entry.Message = Message;
	Pending.Add(MoveTemp(Entry));

	// Accepted into the queue; the outcome is decided at end of tick.
	return true;
}

void USC_RadioEnvironment_SharedChannel::Tick(float DeltaTime)
{
	if (Pending.Num() > 0)
	{
		ResolvePending();
		Pending.Reset();
	}
}

void USC_RadioEnvironment_SharedChannel::ResolvePending() const
{
	UWorld* World = WorldPtr.Get();
	if (!World)
	{
		return;
	}

	const float NoiseMilliwatts = DbmToMilliwatts(NoiseFloorDbm);

	for (const TObjectPtr<USC_ReceiverComponent>& Receiver : Receivers)
	{
		if (Receiver)
		{
			ResolveReceiver(World, Receiver, NoiseMilliwatts);
		}
	}
}

void USC_RadioEnvironment_SharedChannel::ResolveReceiver(UWorld* World, USC_ReceiverComponent* Receiver,
														 float NoiseMilliwatts) const
{
	TMap<int32, FChannelAccumulator> ByChannel;
	GatherSignals(World, Receiver, ByChannel);

	// Each channel is decoded independently.
	for (const TPair<int32, FChannelAccumulator>& Pair : ByChannel)
	{
		DeliverWinner(Receiver, Pair.Key, Pair.Value, NoiseMilliwatts);
	}
}

void USC_RadioEnvironment_SharedChannel::GatherSignals(UWorld* World, USC_ReceiverComponent* Receiver,
													   TMap<int32, FChannelAccumulator>& OutByChannel) const
{
	for (int32 Index = 0; Index < Pending.Num(); ++Index)
	{
		const FSC_RadioBroadcastParams& P = Pending[Index].Params;
		const AActor* SenderActor = P.Sender ? P.Sender->GetOwner() : nullptr;

		float Rssi = 0.0f;
		float DistanceCm = 0.0f;
		bool bObstructed = false;
		if (!ComputeReceivedSignal(World, P, SenderActor, Receiver, Rssi, DistanceCm, bObstructed))
		{
			continue;
		}

		FChannelAccumulator& Acc = OutByChannel.FindOrAdd(P.Channel);
		Acc.TotalMilliwatts += DbmToMilliwatts(Rssi);
		if (Acc.StrongestIndex == INDEX_NONE || Rssi > Acc.StrongestRssiDbm)
		{
			Acc.StrongestIndex = Index;
			Acc.StrongestRssiDbm = Rssi;
			Acc.StrongestDistanceCm = DistanceCm;
			Acc.bStrongestObstructed = bObstructed;
		}
	}
}

void USC_RadioEnvironment_SharedChannel::DeliverWinner(USC_ReceiverComponent* Receiver, int32 Channel,
													   const FChannelAccumulator& Accumulator,
													   float NoiseMilliwatts) const
{
	if (Accumulator.StrongestIndex == INDEX_NONE)
	{
		return;
	}
	if (Accumulator.StrongestRssiDbm < ReceiverSensitivityDbm)
	{
		return;
	}

	const float SignalMilliwatts = DbmToMilliwatts(Accumulator.StrongestRssiDbm);
	const float InterferenceMilliwatts = (Accumulator.TotalMilliwatts - SignalMilliwatts) + NoiseMilliwatts;
	const float SinrDb = Accumulator.StrongestRssiDbm
		- 10.0f * FMath::LogX(10.0f, FMath::Max(InterferenceMilliwatts, KINDA_SMALL_NUMBER));

	if (SinrDb < CaptureThresholdDb)
	{
		return; // Collision: no message is decodable on this channel.
	}

	const FSC_PendingTransmission& Winner = Pending[Accumulator.StrongestIndex];

	FSC_RadioDeliveryInfo DeliveryInfo;
	DeliveryInfo.RssiDbm = Accumulator.StrongestRssiDbm;
	DeliveryInfo.SinrDb = SinrDb;
	DeliveryInfo.Distance = Accumulator.StrongestDistanceCm;
	DeliveryInfo.Channel = Channel;
	DeliveryInfo.bLineOfSightBlocked = Accumulator.bStrongestObstructed;
	DeliveryInfo.Sender = Winner.Params.Sender;

	Receiver->ReceiveMessage(Winner.Message, DeliveryInfo);
}