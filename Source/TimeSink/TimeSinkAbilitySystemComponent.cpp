// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeSinkAbilitySystemComponent.h"

void UTimeSinkAbilitySystemComponent::GetActiveTrackableAbilitiesWithTag(const FGameplayTagContainer& GameplayTagContainer, TArray<UTrackableGameplayAbility*>& ActiveAbilities)
{
	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, AbilitiesToActivate, false);

	// iterate the list of all ability specs:
	for (FGameplayAbilitySpec* Spec : AbilitiesToActivate) {
		// iterate all instances of this ability spec:
		TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

		for (UGameplayAbility* ActiveAbility : AbilityInstances) {
			auto TrackableAbility = Cast<UTrackableGameplayAbility>(ActiveAbility);
			if (TrackableAbility) {
				ActiveAbilities.Add(TrackableAbility);
			}
		}
	}
}

