// Fill out your copyright notice in the Description page of Project Settings.


#include "SC_RadioEnvironment_LineOfSight.h"
#include "SC_ReceiverComponent.h"
#include "SC_TransmitterComponent.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"

void USC_RadioEnvironment_LineOfSight::Initialize(UWorld* World)
{
	WorldPtr = World;
}

void USC_RadioEnvironment_LineOfSight::RegisterReceiver(USC_ReceiverComponent* Receiver)
{
	if (Receiver) { Receivers.AddUnique(Receiver); }
}

void USC_RadioEnvironment_LineOfSight::UnregisterReceiver(USC_ReceiverComponent* Receiver)
{
	Receivers.Remove(Receiver);
}

bool USC_RadioEnvironment_LineOfSight::BroadcastMessage(const FSC_RadioBroadcastParams& Params,
														USC_RadioMessage* Message)
{
	UWorld* World = WorldPtr.Get();
	if (!World)
	{
		return false;
	}

	const AActor* SenderActor = Params.Sender ? Params.Sender->GetOwner() : nullptr;

	bool bDelivered = false;
	for (const TObjectPtr<USC_ReceiverComponent>& Receiver : Receivers)
	{
		if (TryDeliver(World, Params, SenderActor, Receiver, Message))
		{
			bDelivered = true;
		}
	}

	return bDelivered;
}

bool USC_RadioEnvironment_LineOfSight::TryDeliver(UWorld* World, const FSC_RadioBroadcastParams& Params,
												  const AActor* SenderActor, USC_ReceiverComponent* Receiver,
												  USC_RadioMessage* Message) const
{
	float Rssi = 0.0f;
	float DistanceCm = 0.0f;
	bool bObstructed = false;
	if (!ComputeReceivedSignal(World, Params, SenderActor, Receiver, Rssi, DistanceCm, bObstructed))
	{
		return false;
	}

	if (Rssi < ReceiverSensitivityDbm)
	{
		return false;
	}

	FSC_RadioDeliveryInfo DeliveryInfo;
	DeliveryInfo.RssiDbm = Rssi;
	DeliveryInfo.Distance = DistanceCm;
	DeliveryInfo.bLineOfSightBlocked = bObstructed;
	DeliveryInfo.Channel = Params.Channel;
	DeliveryInfo.SinrDb = 0.0f; // Interference is not modelled by this environment.
	DeliveryInfo.Sender = Params.Sender;

	Receiver->ReceiveMessage(Message, DeliveryInfo);
	return true;
}

bool USC_RadioEnvironment_LineOfSight::ComputeReceivedSignal(UWorld* World, const FSC_RadioBroadcastParams& Params,
															 const AActor* SenderActor, USC_ReceiverComponent* Receiver,
															 float& OutRssiDbm, float& OutDistanceCm, bool& bOutObstructed) const
{
	if (!IsValidTarget(Receiver, SenderActor))
	{
		return false;
	}

	const FVector RxLocation = Receiver->GetComponentLocation();
	const float DistanceSq = FVector::DistSquared(RxLocation, Params.Location);
	if (!IsWithinRange(DistanceSq, Params.Radius))
	{
		return false;
	}

	bOutObstructed = IsLineOfSightBlocked(World, Params.Location, RxLocation, SenderActor, Receiver->GetOwner());
	OutDistanceCm = FMath::Sqrt(DistanceSq);
	OutRssiDbm = ComputeRssiDbm(Params.TransmitPowerDbm, OutDistanceCm / 100.0f, bOutObstructed);
	return true;
}

bool USC_RadioEnvironment_LineOfSight::IsValidTarget(const USC_ReceiverComponent* Receiver, const AActor* SenderActor)
{
	if (!Receiver)
	{
		return false;
	}

	// Do not deliver a transmitter's own message to a receiver on the same actor.
	return !(SenderActor && Receiver->GetOwner() == SenderActor);
}

bool USC_RadioEnvironment_LineOfSight::IsWithinRange(float DistanceSq, float Radius)
{
	if (Radius <= 0.0f)
	{
		return true; // No hard range cut-off.
	}
	return DistanceSq <= (Radius * Radius);
}

bool USC_RadioEnvironment_LineOfSight::IsLineOfSightBlocked(UWorld* World, const FVector& From, const FVector& To,
															const AActor* SenderActor, const AActor* ReceiverActor) const
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("RadioLineOfSight")), /*bTraceComplex=*/false);
	if (SenderActor) { TraceParams.AddIgnoredActor(SenderActor); }
	if (ReceiverActor) { TraceParams.AddIgnoredActor(ReceiverActor); }

	return World->LineTraceSingleByChannel(Hit, From, To, LineOfSightChannel.GetValue(), TraceParams);
}

float USC_RadioEnvironment_LineOfSight::ComputeRssiDbm(float TransmitPowerDbm, float DistanceMeters, bool bObstructed) const
{
	// Reference distance is 1 m
	// clamp to avoid a negative/zero logarithm at very short range.
	const float ClampedDistance = FMath::Max(DistanceMeters, 1.0f);
	float PathLoss = ReferencePathLossDb + 10.0f * PathLossExponent * FMath::LogX(10.0f, ClampedDistance);
	if (bObstructed)
	{
		PathLoss += ObstructionLossDb;
	}
	return TransmitPowerDbm - PathLoss;
}