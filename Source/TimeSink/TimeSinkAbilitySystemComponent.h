// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TrackableGameplayAbility.h"
#include "TimeSinkAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class TIMESINK_API UTimeSinkAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void GetActiveTrackableAbilitiesWithTag(const FGameplayTagContainer& GameplayTagContainer, TArray<UTrackableGameplayAbility*>& ActiveAbilities);

};
