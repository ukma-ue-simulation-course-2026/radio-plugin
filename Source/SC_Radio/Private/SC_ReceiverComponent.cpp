// Fill out your copyright notice in the Description page of Project Settings.

#include "SC_ReceiverComponent.h"
#include "SC_RadioWorldSubsystem.h"

USC_ReceiverComponent::USC_ReceiverComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USC_ReceiverComponent::BeginPlay()
{
	Super::BeginPlay();
	if (UWorld* World = GetWorld())
	{
		if (USC_RadioWorldSubsystem* Sub = World->GetSubsystem<USC_RadioWorldSubsystem>())
		{
			Sub->RegisterReceiver(this);
		}
	}
}

void USC_ReceiverComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		if (USC_RadioWorldSubsystem* Sub = World->GetSubsystem<USC_RadioWorldSubsystem>())
		{
			Sub->UnregisterReceiver(this);
		}
	}
	Super::EndPlay(EndPlayReason);
}

bool USC_ReceiverComponent::ReceiveMessage(USC_RadioMessage* Message)
{
	OnMessageReceived.Broadcast(Message);
	return true;
}