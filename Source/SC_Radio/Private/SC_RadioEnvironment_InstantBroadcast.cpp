// Fill out your copyright notice in the Description page of Project Settings.


#include "SC_RadioEnvironment_InstantBroadcast.h"
#include "SC_ReceiverComponent.h" 

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

bool USC_RadioEnvironment_InstantBroadcast::BroadcastMessage(const FVector& Location, float Radius,
															 USC_RadioMessage* Message)
{
	const float RadiusSq = Radius * Radius;
	bool bDelivered = false;

	for (const TObjectPtr<USC_ReceiverComponent>& Receiver : Receivers)
	{
		FVector ReceiverLocation = Receiver->GetComponentLocation();
		const bool bInRadius = FVector::DistSquared(ReceiverLocation, Location) <= RadiusSq;
		
		if (Receiver && bInRadius)
		{
			Receiver->ReceiveMessage(Message);
			bDelivered = true;
		}
	}
	return bDelivered;
}