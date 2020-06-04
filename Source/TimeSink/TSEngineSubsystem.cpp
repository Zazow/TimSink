// Fill out your copyright notice in the Description page of Project Settings.


#include "TSEngineSubsystem.h"
#include "AbilitySystemGlobals.h"

void UTSEngineSubsystem::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);
	UAbilitySystemGlobals::Get().InitGlobalData();
}

