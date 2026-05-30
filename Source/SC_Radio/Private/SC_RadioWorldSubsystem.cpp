// Fill out your copyright notice in the Description page of Project Settings.


#include "SC_RadioWorldSubsystem.h"

#include "SC_RadioSettings.h"

void USC_RadioWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const USC_RadioSettings* Settings = USC_RadioSettings::Get();

	if (!Settings || !Settings->RadioEnvironmentClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("RadioEnvironmentClass is not configured."));
		return;
	}

	// Instantiate the UObject using NewObject with the selected class.
	RadioEnvironmentImplObj = NewObject<UObject>(this, Settings->RadioEnvironmentClass);

	// Verify the interface is actually implemented (defensive check).
	RadioEnvironmentImpl = Cast<ISC_RadioEnvironment>(RadioEnvironmentImplObj);

	if (!RadioEnvironmentImpl)
	{
		UE_LOG(LogTemp, Error, TEXT("RadioEnvironmentClass does not implement ISC_RadioEnvironment."));
		return;
	}

	RadioEnvironmentImpl->Initialize(GetWorld());
}

TStatId USC_RadioWorldSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(
		USC_RadioWorldSubsystem,
		STATGROUP_Tickables
	);
}

bool USC_RadioWorldSubsystem::BroadcastMessage(FVector Location, FSC_RadioMessageHandle Message)
{
	if (RadioEnvironmentImpl == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("RadioEnvironmentImpl is not configured."));
		return false;
	}

	return RadioEnvironmentImpl->BroadcastMessage(Location, Message);
}
