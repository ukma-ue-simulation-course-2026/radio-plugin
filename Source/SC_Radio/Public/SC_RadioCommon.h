// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SC_RadioCommon.generated.h"

USTRUCT()
struct FSC_RadioMessage
{
	GENERATED_BODY();
};

USTRUCT(BlueprintType)
struct FSC_RadioMessageHandle
{
	GENERATED_BODY();

	TSharedPtr<FSC_RadioMessage> Message;
};


USTRUCT()
struct FSC_RadioMessage_String : public FSC_RadioMessage
{
	GENERATED_BODY();

	FString Data;
};
