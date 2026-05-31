#pragma once

#include "CoreMinimal.h"
#include "ISC_RadioEnvironment.h"
#include "UObject/Object.h"
#include "SC_RadioEnvironment_InstantBroadcast.generated.h"

class USC_ReceiverComponent;
class USC_RadioMessage;

UCLASS()
class SC_RADIO_API USC_RadioEnvironment_InstantBroadcast : public UObject, public ISC_RadioEnvironment
{
	GENERATED_BODY()

public:
	virtual void Initialize(UWorld* World) override;
	virtual void RegisterReceiver(USC_ReceiverComponent* Receiver) override;
	virtual void UnregisterReceiver(USC_ReceiverComponent* Receiver) override;
	virtual bool BroadcastMessage(const FVector& Location, float Radius, USC_RadioMessage* Message) override;

private:
	UPROPERTY()
	TArray<TObjectPtr<USC_ReceiverComponent>> Receivers;
};