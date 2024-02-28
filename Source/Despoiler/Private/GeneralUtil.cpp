// Fill out your copyright notice in the Description page of Project Settings.

#include "GeneralUtil.h"
#include "Squad.h"

AActor* UGeneralUtil::GetClosestActor(FVector location, TArray<AActor*> actors)
{
	AActor* closestActor = nullptr;
	float closestDistance = 10000000000;
	for (AActor* actor : actors)
	{
		if (actor == nullptr) continue;
		FVector actorLocation = actor->GetActorLocation();
		float distance = FVector::Dist(location, actorLocation);
		//UE_LOG(LogTemp, Warning, TEXT("actorLocation: %s, %f"), *actorLocation.ToString(), distance);
		if (distance < closestDistance)
		{
			closestActor = actor;
			closestDistance = distance;
		}
	}
	return closestActor;
}

bool UGeneralUtil::IsSquadStateEqual(ASquad* squad, ESquadState goalState)
{
	if (squad->SquadBlackboard != nullptr)
	{
		return squad->SquadBlackboard->SquadState == goalState;
	}

	return false;
}
