#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISC_RadioEnvironment.generated.h"

class USC_ReceiverComponent;
class USC_RadioMessage;

UINTERFACE(MinimalAPI)
class USC_RadioEnvironment : public UInterface
{
	GENERATED_BODY()
};

class SC_RADIO_API ISC_RadioEnvironment
{
	GENERATED_BODY()

public:
	virtual void Initialize(UWorld* World) = 0;

	virtual void RegisterReceiver(USC_ReceiverComponent* Receiver) = 0;
	virtual void UnregisterReceiver(USC_ReceiverComponent* Receiver) = 0;

	// Location -> transmitter location, Radius -> broadcast sphere
	virtual bool BroadcastMessage(const FVector& Location, float Radius, USC_RadioMessage* Message) = 0;
};