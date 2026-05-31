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

	UPROPERTY(BlueprintReadWrite)
	float Radius = 1000.0f;

	UFUNCTION(BlueprintCallable)
	virtual bool SendMessage(USC_RadioMessage* Message);
};