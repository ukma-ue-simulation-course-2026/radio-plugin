// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SC_RadioSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig)
class SC_RADIO_API USC_RadioSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	USC_RadioSettings();
	
	static const USC_RadioSettings* Get();
	
	UPROPERTY(Config, EditAnywhere, meta = (MustImplement = "/Script/SC_Radio.RadioEnviroment"))
	TSubclassOf<UObject> RadioEnvironmentClass;
};
