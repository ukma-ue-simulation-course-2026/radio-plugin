// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ISC_RadioEnvironment.h"
#include "Subsystems/WorldSubsystem.h"
#include "SC_RadioWorldSubsystem.generated.h"

class USC_ReceiverComponent;
class USC_RadioMessage;

UCLASS()
class SC_RADIO_API USC_RadioWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void RegisterReceiver(USC_ReceiverComponent* Receiver);
	void UnregisterReceiver(USC_ReceiverComponent* Receiver);

	bool BroadcastMessage(const FVector& Location, float Radius, USC_RadioMessage* Message);

private:
	UPROPERTY()
	TScriptInterface<ISC_RadioEnvironment> Environment;
};