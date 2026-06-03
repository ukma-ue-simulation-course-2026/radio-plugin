#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SC_RadioCommon.h"
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

	// Submits Message for propagation
	// Environments may deliver immediately or defer to Tick
	// return value indicates the message was accepted, not necessarily delivered
	virtual bool BroadcastMessage(const FSC_RadioBroadcastParams& Params, USC_RadioMessage* Message) = 0;

	// Advances the environment
	// Synchronous environments may leave this empty
	// deferred environments resolve their queued transmissions here
	virtual void Tick(float DeltaTime) {}
};