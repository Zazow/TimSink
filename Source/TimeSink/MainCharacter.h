// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "TimeSinkAbilitySystemComponent.h"
#include "GameplayCueInterface.h"
#include "MainCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_TwoParams(FSwordOverlapActorSignature, AMainCharacter, OnSwordOverlapActor, AActor*, OtherActor, const FHitResult&, HitResult);

class UInputComponent;

//It's very important that this enum is UENUM, because the code will look for UENUM by the given name and crash if the UENUM can't be found. BlueprintType is there so we can use these in blueprints, too. Just in case. Can be neat to define ability packages.
UENUM(BlueprintType)
enum class AbilityInput : uint8
{
	EAI_PRIMARY UMETA(DisplayName = "Primary Attack"), //This maps the first ability(input ID should be 0 in int) to the action mapping(which you define in the project settings) by the name of "UseAbility1". "Use Spell 1" is the blueprint name of the element.

	//You may also do something like define an enum element name that is not actually mapped to an input, for example if you have a passive ability that isn't supposed to have an input. This isn't usually necessary though as you usually grant abilities via input ID,
	//which can be negative while enums cannot. In fact, a constant called "INDEX_NONE" exists for the exact purpose of rendering an input as unavailable, and it's simply defined as -1.
	//Because abilities are granted by input ID, which is an int, you may use enum elements to describe the ID anyway however, because enums are fancily dressed up ints.
};

UCLASS()
class TIMESINK_API AMainCharacter : public ACharacter, public IAbilitySystemInterface, public IGameplayCueInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	TSubclassOf<class UGameplayAbility> PrimaryAttackAbility;

	/** Ability System */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	UTimeSinkAbilitySystemComponent* AbilitySystem;

	UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystem; };

	// COMBAT

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	UStaticMeshComponent* RightSword;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	UStaticMeshComponent* LeftSword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	TEnumAsByte<ECollisionChannel> SwordCollisionChannel;
	
	UPROPERTY(BlueprintAssignable, Category = Combat)
	FSwordOverlapActorSignature OnSwordOverlapActor;

	// Tracing variables
	FTransform RightSwordLastTransform;
	FTransform LeftSwordLastTransform;
	FCollisionObjectQueryParams TraceParams;
	FCollisionQueryParams QueryParams;
	TSet<AActor*> RightSwordHits;
	TSet<AActor*> LeftSwordHits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	int TracePoints = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	float TraceLength = 10;

	float SpaceBetweenTracePoints = TraceLength/TracePoints;

	UFUNCTION(BlueprintCallable)
	void StartDamaging();
	UFUNCTION(BlueprintCallable)
	void StopDamaging();

	void TraceSwords(float DeltaTime);



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void Restart() override;

	// This function is called whenever a property is changed from withing a blueprint.
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	// This array holds function that need to be called every tick.
	TArray<void(AMainCharacter::*)(float DeltaTime)> ThingsToUpdate;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))

	class UCameraComponent* FirstPersonCameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "CharacterAttributes")
	class UCharacterAttributeSet* CharacterAttributeComponent;
	
protected:
	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Fires a projectile. */
	void Attack();
};
