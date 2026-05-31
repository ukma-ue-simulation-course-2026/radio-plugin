#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SC_ReceiverComponent.generated.h"

class USC_RadioMessage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRadioMessageReceived, USC_RadioMessage*, Message);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SC_RADIO_API USC_ReceiverComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	USC_ReceiverComponent();

	// Bind to react to messages
	UPROPERTY(BlueprintAssignable)
	FOnRadioMessageReceived OnMessageReceived;

	virtual bool ReceiveMessage(USC_RadioMessage* Message);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};