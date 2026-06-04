// Fill out your copyright notice in the Description page of Project Settings.


#include "SC_RadioEnvironment_InstantBroadcast.h"
#include "SC_ReceiverComponent.h"
#include "SC_TransmitterComponent.h"

void USC_RadioEnvironment_InstantBroadcast::Initialize(UWorld* World)
{
}

void USC_RadioEnvironment_InstantBroadcast::RegisterReceiver(USC_ReceiverComponent* Receiver)
{
	if (Receiver) { Receivers.AddUnique(Receiver); }
}

void USC_RadioEnvironment_InstantBroadcast::UnregisterReceiver(USC_ReceiverComponent* Receiver)
{
	Receivers.Remove(Receiver);
}

bool USC_RadioEnvironment_InstantBroadcast::BroadcastMessage(const FSC_RadioBroadcastParams& Params,
															 USC_RadioMessage* Message)
{
	const float RadiusSq = Params.Radius * Params.Radius;
	const AActor* SenderActor = Params.Sender ? Params.Sender->GetOwner() : nullptr;
	bool bDelivered = false;

	for (const TObjectPtr<USC_ReceiverComponent>& Receiver : Receivers)
	{
		if (!Receiver)
		{
			continue;
		}

		// Do not deliver a transmitter's own message to a receiver on the same actor
		if (SenderActor && Receiver->GetOwner() == SenderActor)
		{
			continue;
		}

		const FVector ReceiverLocation = Receiver->GetComponentLocation();
		const float DistanceSq = FVector::DistSquared(ReceiverLocation, Params.Location);
		if (DistanceSq > RadiusSq)
		{
			continue;
		}

		FSC_RadioDeliveryInfo DeliveryInfo;
		DeliveryInfo.Distance = FMath::Sqrt(DistanceSq);
		DeliveryInfo.RssiDbm = 0.0f; // Signal strength is not modelled by this environment
		DeliveryInfo.Sender = Params.Sender;

		Receiver->ReceiveMessage(Message, DeliveryInfo);
		bDelivered = true;
	}

	return bDelivered;
}