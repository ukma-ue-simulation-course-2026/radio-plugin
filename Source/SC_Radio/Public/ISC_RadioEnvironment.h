// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SC_RadioCommon.h"
#include "UObject/Interface.h"
#include "ISC_RadioEnvironment.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USC_RadioEnvironment : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SC_RADIO_API ISC_RadioEnvironment
{
	GENERATED_BODY()

public:
	// Location -> transmitter location
	virtual bool BroadcastMessage(FVector Location, FSC_RadioMessageHandle Message) = 0;
	virtual bool Initialize(UWorld* World) = 0;
};
