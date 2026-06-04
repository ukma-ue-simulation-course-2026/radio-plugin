// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ISC_RadioEnvironment.h"
#include "UObject/Object.h"
#include "Engine/EngineTypes.h"
#include "SC_RadioEnvironment_LineOfSight.generated.h"

class AActor;
class USC_ReceiverComponent;
class USC_RadioMessage;

/**
 * Distance- and obstruction-aware environment.
 *
 * Delivery is gated by a log-distance path-loss RSSI model: a message is
 * received only when its estimated signal strength meets the receiver
 * sensitivity threshold. World geometry between transmitter and receiver
 * adds a configurable obstruction penalty.
 *
 * Subclass this in Blueprint to expose tuned presets (urban, open field, etc.)
 * and select the result in Project Settings -> Radio. The protected signal
 * helpers are intended for reuse by derived environments.
 */
UCLASS(Blueprintable)
class SC_RADIO_API USC_RadioEnvironment_LineOfSight : public UObject, public ISC_RadioEnvironment
{
	GENERATED_BODY()

public:
	virtual void Initialize(UWorld* World) override;
	virtual void RegisterReceiver(USC_ReceiverComponent* Receiver) override;
	virtual void UnregisterReceiver(USC_ReceiverComponent* Receiver) override;
	virtual bool BroadcastMessage(const FSC_RadioBroadcastParams& Params, USC_RadioMessage* Message) override;

protected:
	// Path loss (dB) at the one-metre reference distance.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReferencePathLossDb = 40.0f;

	// Log-distance path-loss exponent (2.0 = free space, 2.7-3.5 = urban).
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PathLossExponent = 2.0f;

	// Receiver sensitivity (dBm). Messages below this RSSI are dropped.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReceiverSensitivityDbm = -90.0f;

	// Extra attenuation (dB) applied when world geometry blocks line of sight.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ObstructionLossDb = 30.0f;

	// Trace channel used for the line-of-sight check.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ECollisionChannel> LineOfSightChannel = ECC_Visibility;
	
	bool ComputeReceivedSignal(UWorld* World, const FSC_RadioBroadcastParams& Params, const AActor* SenderActor,
							   USC_ReceiverComponent* Receiver,
							   float& OutRssiDbm, float& OutDistanceCm, bool& bOutObstructed) const;

	bool IsLineOfSightBlocked(UWorld* World, const FVector& From, const FVector& To,
							  const AActor* SenderActor, const AActor* ReceiverActor) const;

	float ComputeRssiDbm(float TransmitPowerDbm, float DistanceMeters, bool bObstructed) const;

	static bool IsValidTarget(const USC_ReceiverComponent* Receiver, const AActor* SenderActor);

	static bool IsWithinRange(float DistanceSq, float Radius);

	UPROPERTY()
	TArray<TObjectPtr<USC_ReceiverComponent>> Receivers;

	TWeakObjectPtr<UWorld> WorldPtr;

private:
	bool TryDeliver(UWorld* World, const FSC_RadioBroadcastParams& Params, const AActor* SenderActor,
					USC_ReceiverComponent* Receiver, USC_RadioMessage* Message) const;
};