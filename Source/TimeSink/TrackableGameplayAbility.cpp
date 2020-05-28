// Fill out your copyright notice in the Description page of Project Settings.


#include "TrackableGameplayAbility.h"

void UTrackableGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	OnAbilityEnded.Broadcast(bWasCancelled);
}