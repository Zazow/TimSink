// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TrackableGameplayAbility.generated.h"

// custom delegate that gets called when this ability ended execution
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FAbilityEndedSignature, UTrackableGameplayAbility, OnAbilityEnded, bool, Cancelled);


/**
 * 
 */
UCLASS()
class TIMESINK_API UTrackableGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()


    /** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

public:
    UPROPERTY(BlueprintAssignable)
    FAbilityEndedSignature OnAbilityEnded;
	
};
