// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SC_RadioCommon.generated.h"

class USC_TransmitterComponent;

UCLASS(Abstract, BlueprintType, EditInlineNew)
class SC_RADIO_API USC_RadioMessage : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual FString ToString() const;
};

UCLASS(BlueprintType)
class SC_RADIO_API USC_RadioMessage_String : public USC_RadioMessage
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FString Data;

	virtual FString ToString() const override;
};

USTRUCT(BlueprintType)
struct SC_RADIO_API FSC_RadioBroadcastParams
{
	GENERATED_BODY()

	// transmitter location
	UPROPERTY(BlueprintReadWrite)
	FVector Location = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	float Radius = 0.0f;

	// used by RSSI-aware environments
	UPROPERTY(BlueprintReadWrite)
	float TransmitPowerDbm = 20.0f;

	// logical channel/frequency
	UPROPERTY(BlueprintReadWrite)
	int32 Channel = 0;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<USC_TransmitterComponent> Sender = nullptr;
};

/** Per-receiver metadata produced when a message is delivered. */
USTRUCT(BlueprintType)
struct SC_RADIO_API FSC_RadioDeliveryInfo
{
	GENERATED_BODY()

	// received signal strength in dBm
	UPROPERTY(BlueprintReadWrite)
	float RssiDbm = 0.0f;

	// signal-to-interference-plus-noise ratio in dB. Only meaningful for channel-aware environments.
	UPROPERTY(BlueprintReadWrite)
	float SinrDb = 0.0f;

	// straight-line transmitter-to-receiver distance in centimetres
	UPROPERTY(BlueprintReadWrite)
	float Distance = 0.0f;

	// the message was received on
	UPROPERTY(BlueprintReadWrite)
	int32 Channel = 0;

	// True when world geometry blocked the direct path (line-of-sight environments)
	UPROPERTY(BlueprintReadWrite)
	bool bLineOfSightBlocked = false;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<USC_TransmitterComponent> Sender = nullptr;
};