#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SC_TransmitterComponent.generated.h"

class USC_RadioMessage;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SC_RADIO_API USC_TransmitterComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	USC_TransmitterComponent();

	// Optional hard range cut-off in centimetres
	// Values <= 0 disable the cut-off.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Radius = 1000.0f;

	// Transmit power in dBm
	// consumed by RSSI-aware environments.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TransmitPowerDbm = 20.0f;

	// Logical channel/frequency
	// Only transmitters sharing a channel interfere with one another.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Channel = 0;

	// Submits a message to the active radio environment.
	// return value reports acceptance
	// with deferred environments (e.g. shared-channel) the actual delivery is decided later
	// and surfaces through USC_ReceiverComponent::OnMessageReceived
	UFUNCTION(BlueprintCallable)
	virtual bool SendMessage(USC_RadioMessage* Message);
};