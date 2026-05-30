// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ISC_RadioEnvironment.h"
#include "UObject/Object.h"
#include "SC_RadioEnvironment_InstantBroadcast.generated.h"

/**
 * 
 */
UCLASS()
class SC_RADIO_API USC_RadioEnvironment_InstantBroadcast : public UObject, public ISC_RadioEnvironment
{
	GENERATED_BODY()

public:
	bool Initialize(UWorld* World) override;
	bool BroadcastMessage(FVector Location, FSC_RadioMessageHandle Message) override;
	
	
};
