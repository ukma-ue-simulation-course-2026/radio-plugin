// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SC_RadioCommon.generated.h"

UCLASS(Abstract, BlueprintType, EditInlineNew)
class SC_RADIO_API USC_RadioMessage : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Radio")
	virtual FString ToString() const;
};

UCLASS(BlueprintType)
class SC_RADIO_API USC_RadioMessage_String : public USC_RadioMessage
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio")
	FString Data;

	virtual FString ToString() const override;
};