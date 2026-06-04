// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldSubsystem/SC_RadioWorldSubsystem.h"

#include "SC_RadioSettings.h"
#include "EnvironmentImpl/SC_RadioEnvironment_InstantBroadcast.h"

void USC_RadioWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	TSubclassOf<UObject> EnvClass = USC_RadioSettings::Get()->RadioEnvironmentClass;
	if (!EnvClass || !EnvClass->ImplementsInterface(USC_RadioEnvironment::StaticClass()))
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

void USC_RadioWorldSubsystem::Tick(float DeltaTime)
{
	if (Environment)
	{
		Environment->Tick(DeltaTime);
	}
}

TStatId USC_RadioWorldSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(USC_RadioWorldSubsystem, STATGROUP_Tickables);
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

bool USC_RadioWorldSubsystem::BroadcastMessage(const FSC_RadioBroadcastParams& Params, USC_RadioMessage* Message)
{
	return Environment
		? Environment->BroadcastMessage(Params, Message)
		: false;
}