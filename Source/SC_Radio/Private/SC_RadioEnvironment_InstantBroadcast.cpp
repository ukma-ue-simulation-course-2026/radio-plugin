// Fill out your copyright notice in the Description page of Project Settings.


#include "SC_RadioEnvironment_InstantBroadcast.h"

bool USC_RadioEnvironment_InstantBroadcast::Initialize(UWorld* World)
{
	return true;
}

bool USC_RadioEnvironment_InstantBroadcast::BroadcastMessage(FVector Location, FSC_RadioMessageHandle Message)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, Location.ToString());
	return true;
}
