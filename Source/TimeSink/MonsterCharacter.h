// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "TimeSinkAbilitySystemComponent.h"
#include "MonsterCharacter.generated.h"

UCLASS()
class TIMESINK_API AMonsterCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonsterCharacter();

	/** Ability System */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	class UTimeSinkAbilitySystemComponent* AbilitySystem;

	UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystem; };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	TSubclassOf<class UGameplayAbility> PrimaryAttackAbility;

	UFUNCTION(BlueprintCallable)
	void GetActiveTrackableAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<class UTrackableGameplayAbility*>& ActiveAbilities);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PerformPrimaryAttack(UTrackableGameplayAbility* Ability);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
