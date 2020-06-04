// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTask_WaitHitFromCharacter.h"

UAbilityTask_WaitHitFromCharacter* UAbilityTask_WaitHitFromCharacter::WaitHitFromCharacter(UGameplayAbility* OwningAbility, FName TaskInstanceName, AMainCharacter MainCharacter, bool bOnlyTriggerOnce)
{
	auto MyTask = NewAbilityTask<UAbilityTask_WaitHitFromCharacter>(OwningAbility, TaskInstanceName);
	MyTask->MainCharacter = MainCharacter;
	MyTask->bOnlyTriggerOnce = bOnlyTriggerOnce;
	return MyTask;
}

void UAbilityTask_WaitHitFromCharacter::Activate()
{
	if (IsPendingKill()) {
		return;
	}

	if (Ability && MainCharacter) {

	}
	else {
		EndTask();
	}
}

void UAbilityTask_WaitHitFromCharacter::OnHitRegistered(AActor* Other, FHitResult HitResult)
{
}

void UAbilityTask_WaitHitFromCharacter::ExternalConfirm(bool bEndTask)
{
}

void UAbilityTask_WaitHitFromCharacter::ExternalCancel()
{
}
