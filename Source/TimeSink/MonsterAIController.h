// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class TIMESINK_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSet<TSubclassOf<AActor>> HostileClasses;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSet<TSubclassOf<AActor>> FriendlyClasses;

	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;	
};
