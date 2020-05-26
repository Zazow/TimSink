// Copyright Epic Games, Inc. All Rights Reserved.

#include "TimeSinkGameMode.h"
#include "TimeSinkHUD.h"
#include "TimeSinkCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATimeSinkGameMode::ATimeSinkGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATimeSinkHUD::StaticClass();
}
