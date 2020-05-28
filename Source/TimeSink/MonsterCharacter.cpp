// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterCharacter.h"

// Sets default values
AMonsterCharacter::AMonsterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystem = CreateDefaultSubobject<UTimeSinkAbilitySystemComponent>(TEXT("AbilitySystem"));
}

void AMonsterCharacter::PerformPrimaryAttack_Implementation(UTrackableGameplayAbility* Ability)
{
	UE_LOG(LogTemp, Warning, TEXT("You are calling the abstract implementation of primary attack."));
}

void AMonsterCharacter::GetActiveTrackableAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<class UTrackableGameplayAbility*>& ActiveAbilities)
{
	if (AbilitySystem) {
		AbilitySystem->GetActiveTrackableAbilitiesWithTag(AbilityTags, ActiveAbilities);
	}
}

// Called when the game starts or when spawned
void AMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority()) {
		if (PrimaryAttackAbility) {
			AbilitySystem->GiveAbility(FGameplayAbilitySpec(PrimaryAttackAbility.GetDefaultObject(), 1, 0));
		}
	}
}

void AMonsterCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AbilitySystem->InitAbilityActorInfo(this, this);
	AbilitySystem->RefreshAbilityActorInfo();
}

// Called every frame
void AMonsterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMonsterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

