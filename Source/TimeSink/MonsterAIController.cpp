// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"

ETeamAttitude::Type AMonsterAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (HostileClasses.Contains(Other.GetClass())) {
		return ETeamAttitude::Hostile;
	}

	if (FriendlyClasses.Contains(Other.GetClass())) {
		return ETeamAttitude::Friendly;
	}

	return ETeamAttitude::Neutral;
}
