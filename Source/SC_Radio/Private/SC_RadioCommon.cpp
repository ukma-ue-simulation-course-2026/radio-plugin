// Fill out your copyright notice in the Description page of Project Settings.

#include "SC_RadioCommon.h"

FString USC_RadioMessage::ToString() const
{
	return TEXT("RadioMessage (base)");
}

FString USC_RadioMessage_String::ToString() const
{
	return FString::Printf(TEXT("RadioMessage_String: %s"), *Data);
}