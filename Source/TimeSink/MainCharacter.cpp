// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h" // maybe not needed
#include "MotionControllerComponent.h" // maybe not needed
#include "GameFramework/SpringArmComponent.h"
#include "CharacterAttributeSet.h"
#include "GameplayEffect.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(CameraBoom);
	//FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	//FirstPersonCameraComponent->bUsePawnControlRotation = true;

	AbilitySystem = CreateDefaultSubobject<UTimeSinkAbilitySystemComponent>(TEXT("AbilitySystem"));
	AbilitySystem->ReplicationMode = EGameplayEffectReplicationMode::Mixed;

	CharacterAttributeComponent = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("CharacterAttributeComponent"));

	RightSword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightSwordMesh"));
	RightSword->SetupAttachment(GetMesh(), FName(TEXT("hand_rSocket")));
	LeftSword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftSwordMesh"));
	LeftSword->SetupAttachment(GetMesh(), FName(TEXT("hand_lSocket")));
}

void AMainCharacter::StartDamaging()
{
	// todo: see if this should only activate on a locally controlled character or not.
	ThingsToUpdate.AddUnique(&AMainCharacter::TraceSwords);
	RightSwordLastTransform = RightSword->GetComponentTransform();
	LeftSwordLastTransform = LeftSword->GetComponentTransform();

	RightSwordHits.Empty();
	LeftSwordHits.Empty();
}

void AMainCharacter::StopDamaging()
{
	ThingsToUpdate.Remove(&AMainCharacter::TraceSwords);
}

void AMainCharacter::TraceSwords(float DeltaTime)
{
	auto RightSwordCurrentTransform = RightSword->GetComponentTransform();
	// Right sword trace:
	for (int i = 0; i < TracePoints + 1; i++) {
		// Trace from the last point to this current point
		auto TraceStart = RightSwordLastTransform.GetLocation() + RightSwordLastTransform.GetRotation().GetUpVector() * i * SpaceBetweenTracePoints;
		auto TraceEnd = RightSwordCurrentTransform.GetLocation() + RightSwordCurrentTransform.GetRotation().GetUpVector() * i * SpaceBetweenTracePoints;
		FHitResult OutHit;
		if (GetWorld()->LineTraceSingleByObjectType(OutHit, TraceStart, TraceEnd, TraceParams, QueryParams)) {
			auto Other = OutHit.GetActor();
			if (Other && !RightSwordHits.Contains(Other)) {
				OnSwordOverlapActor.Broadcast(Other, OutHit);
				RightSwordHits.Add(Other);
			}
		}
	}
	RightSwordLastTransform = RightSwordCurrentTransform;

	auto LeftSwordCurrentTransform = LeftSword->GetComponentTransform();
	// Left sword trace:
	for (int i = 0; i < TracePoints + 1; i++) {
		// Trace from the last point to this current point
		auto TraceStart = LeftSwordLastTransform.GetLocation() + LeftSwordLastTransform.GetRotation().GetUpVector() * i * SpaceBetweenTracePoints;
		auto TraceEnd = LeftSwordCurrentTransform.GetLocation() + LeftSwordCurrentTransform.GetRotation().GetUpVector() * i * SpaceBetweenTracePoints;
		FHitResult OutHit;
		if (GetWorld()->LineTraceSingleByObjectType(OutHit, TraceStart, TraceEnd, TraceParams, QueryParams)) {
			auto Other = OutHit.GetActor();
			if (Other && !LeftSwordHits.Contains(Other)) {
				OnSwordOverlapActor.Broadcast(Other, OutHit);
				LeftSwordHits.Add(Other);
			}
		}
	}
	LeftSwordLastTransform = LeftSwordCurrentTransform;

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority()) {
		if (PrimaryAttackAbility) {
			AbilitySystem->GiveAbility(FGameplayAbilitySpec(PrimaryAttackAbility.GetDefaultObject(), 1, 0));
		}
	}

	AbilitySystem->InitAbilityActorInfo(this, this);

	QueryParams.AddIgnoredActor(this);
	QueryParams.TraceTag = FName(TEXT("MainCharacter"));

	SpaceBetweenTracePoints = TraceLength / TracePoints;

	TraceParams = FCollisionObjectQueryParams();
	TraceParams.AddObjectTypesToQuery(SwordCollisionChannel);
}

void AMainCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AbilitySystem->RefreshAbilityActorInfo();

}

void AMainCharacter::Restart()
{
	Super::Restart();
	AbilitySystem->RefreshAbilityActorInfo();
}

void AMainCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetPropertyName() == FName(TEXT("TracePoints")) || PropertyChangedEvent.GetPropertyName() == FName(TEXT("TraceLength"))) {
		SpaceBetweenTracePoints = TraceLength / TracePoints;
	}
	if (PropertyChangedEvent.GetPropertyName() == FName(TEXT("SwordCollisionChannel"))) {
		// reset params before adding an object type.
		TraceParams = FCollisionObjectQueryParams();
		TraceParams.AddObjectTypesToQuery(SwordCollisionChannel);
	}

}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// call functions in ThingsToUpdate
	for (int i = 0; i < ThingsToUpdate.Num(); i++) {
		(this->*(ThingsToUpdate[i]))(DeltaTime);
	}
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
		// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMainCharacter::Attack);

	// Enable touchscreen input
	//EnableTouchscreenMovement(PlayerInputComponent);

	//PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ATimeSinkCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMainCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMainCharacter::LookUpAtRate);

	AbilitySystem->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbilityInputBinds("ConfirmInput", "CancelInput", "AbilityInput"));

}

void AMainCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AMainCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AMainCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::Attack() 
{
	//do nothing
}