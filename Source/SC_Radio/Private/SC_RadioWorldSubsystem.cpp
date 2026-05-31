// Fill out your copyright notice in the Description page of Project Settings.

#include "SC_RadioWorldSubsystem.h"
#include "SC_RadioEnvironment_InstantBroadcast.h"
#include "SC_RadioSettings.h"

void USC_RadioWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	TSubclassOf<UObject> EnvClass = USC_RadioSettings::Get()->RadioEnvironmentClass;
	if (!EnvClass)
	{
		EnvClass = USC_RadioEnvironment_InstantBroadcast::StaticClass();
	}

	UObject* EnvObject = NewObject<UObject>(this, EnvClass);
	Environment.SetObject(EnvObject);
	Environment.SetInterface(Cast<ISC_RadioEnvironment>(EnvObject));

	if (Environment)
	{
		Environment->Initialize(GetWorld());
	}
}

void USC_RadioWorldSubsystem::RegisterReceiver(USC_ReceiverComponent* Receiver)
{
	if (Environment)
	{
		Environment->RegisterReceiver(Receiver);
	}
}

void USC_RadioWorldSubsystem::UnregisterReceiver(USC_ReceiverComponent* Receiver)
{
	if (Environment)
	{
		Environment->UnregisterReceiver(Receiver);
	}
}

bool USC_RadioWorldSubsystem::BroadcastMessage(const FVector& Location, float Radius, USC_RadioMessage* Message)
{
	return Environment
		? Environment->BroadcastMessage(Location, Radius, Message)
		: false;
}