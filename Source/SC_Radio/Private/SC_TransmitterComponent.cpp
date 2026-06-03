// Fill out your copyright notice in the Description page of Project Settings.

#include "SC_TransmitterComponent.h"
#include "SC_RadioCommon.h"
#include "SC_RadioWorldSubsystem.h"

USC_TransmitterComponent::USC_TransmitterComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool USC_TransmitterComponent::SendMessage(USC_RadioMessage* Message)
{
	const UWorld* World = GetWorld();
	
	if (!World)
	{
		return false;
	}
	
	if (USC_RadioWorldSubsystem* Sub = World->GetSubsystem<USC_RadioWorldSubsystem>())
	{
		FSC_RadioBroadcastParams Params;
		Params.Location = GetComponentLocation();
		Params.Radius = Radius;
		Params.TransmitPowerDbm = TransmitPowerDbm;
		Params.Sender = this;

		return Sub->BroadcastMessage(Params, Message);
	}
	return false;
}
