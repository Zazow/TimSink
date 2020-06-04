// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "MainCharacter.h"
#include "AbilityTask_WaitHitFromCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParam(FWaitHitFromCharacterDelegate, const AActor* HitActor, const FHitResult&, HitResult);

/**
 * Waits for a hit from a AMainCharacter instance.
 */

UCLASS()
class TIMESINK_API UAbilityTask_WaitHitFromCharacter : public UAbilityTask
{
	GENERATED_BODY()
	UPROPERTY(BlueprintAssignable)
	FWaitHitFromCharacterDelegate OnHit;

	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true", HideSpawnParms = "Instigator"), Category = "Ability|Tasks")
		static UAbilityTask_WaitHitFromCharacter* WaitHitFromCharacter(
			UGameplayAbility* OwningAbility,
			FName TaskInstanceName,
			AMainCharacter MainCharacter,
			bool bOnlyTriggerOnce
		);

	virtual void Activate() override;

	UFUNCTION()
	virtual void OnHitRegistered(AActor* Other, FHitResult HitResult);

	// Called when the ability is asked to confirm from an outside node. What this means depends on the individual task. By default, this does nothing other than ending if bEndTask is true.
	virtual void ExternalConfirm(bool bEndTask) override;

	// Called when the ability is asked to cancel from an outside node. What this means depends on the individual task. By default, this does nothing other than ending the task.
	virtual void ExternalCancel() override;

protected:
	AMainCharacter* MainCharacter;
	bool bOnlyTriggerOnce;
};
