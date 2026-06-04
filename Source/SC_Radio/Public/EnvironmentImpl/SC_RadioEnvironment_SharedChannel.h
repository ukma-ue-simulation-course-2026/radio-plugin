// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SC_RadioEnvironment_LineOfSight.h"
#include "SC_RadioEnvironment_SharedChannel.generated.h"

class USC_RadioMessage;

/** A transmission queued during the frame, awaiting end-of-tick resolution. */
USTRUCT()
struct FSC_PendingTransmission
{
	GENERATED_BODY()

	UPROPERTY()
	FSC_RadioBroadcastParams Params;

	UPROPERTY()
	TObjectPtr<USC_RadioMessage> Message = nullptr;
};

/**
 * Shared-medium environment built on the line-of-sight model.
 *
 * Broadcasts are not delivered immediately; they are queued and resolved together
 * at the end of the frame in Tick. For each receiver, signals are grouped by channel.
 * Within a channel only the strongest signal can be decoded, and only if it clears
 * both the receiver sensitivity and the capture margin against the combined power of
 * the competing signals plus background noise (SINR). Otherwise the channel collides
 * and nothing is delivered. Transmissions on different channels never interfere.
 */
UCLASS(Blueprintable)
class SC_RADIO_API USC_RadioEnvironment_SharedChannel : public USC_RadioEnvironment_LineOfSight
{
	GENERATED_BODY()

public:
	// Queues the transmission. Returns true once accepted; delivery is decided in Tick.
	virtual bool BroadcastMessage(const FSC_RadioBroadcastParams& Params, USC_RadioMessage* Message) override;

	// Resolves every transmission queued during the frame, then clears the queue.
	virtual void Tick(float DeltaTime) override;

protected:
	// Background noise floor (dBm) used when computing SINR.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NoiseFloorDbm = -100.0f;

	// Minimum SINR (dB) for the strongest signal on a channel to be decoded (capture effect).
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CaptureThresholdDb = 10.0f;

private:
	// Per-receiver, per-channel running totals gathered while scanning queued transmissions.
	struct FChannelAccumulator
	{
		float TotalMilliwatts = 0.0f;
		int32 StrongestIndex = INDEX_NONE;
		float StrongestRssiDbm = 0.0f;
		float StrongestDistanceCm = 0.0f;
		bool bStrongestObstructed = false;
	};

	// Resolves all queued transmissions for every registered receiver.
	void ResolvePending() const;

	// Resolves the queue for a single receiver: gather signals, then decode each channel.
	void ResolveReceiver(UWorld* World, USC_ReceiverComponent* Receiver, float NoiseMilliwatts) const;

	// Accumulates every queued signal that reaches the receiver, grouped by channel.
	void GatherSignals(UWorld* World, USC_ReceiverComponent* Receiver,
					   TMap<int32, FChannelAccumulator>& OutByChannel) const;

	// Applies the capture/SINR rule for one channel and delivers the winning message if any.
	void DeliverWinner(USC_ReceiverComponent* Receiver, int32 Channel,
					   const FChannelAccumulator& Accumulator, float NoiseMilliwatts) const;

	static float DbmToMilliwatts(float Dbm);

	UPROPERTY()
	TArray<FSC_PendingTransmission> Pending;
};