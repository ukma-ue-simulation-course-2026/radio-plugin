// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ISC_RadioEnvironment.h"
#include "Subsystems/WorldSubsystem.h"
#include "SC_RadioWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SC_RADIO_API USC_RadioWorldSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual TStatId GetStatId() const override;
	
	ISC_RadioEnvironment* RadioEnvironmentImpl;

	UFUNCTION(BlueprintCallable)
	bool BroadcastMessage(FVector Location, FSC_RadioMessageHandle Message);

private:
	// To ensure that GC does not remove the Impl instance
	UPROPERTY()
	UObject* RadioEnvironmentImplObj = nullptr;
};
