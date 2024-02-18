// Fill out your copyright notice in the Description page of Project Settings.


#include "Team.h"

// Sets default values
ATeam::ATeam()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

AActor* ATeam::GetClosestMember(FVector location)
{
	AActor* closestMember = nullptr;
	float closestDistance = 10000000000;
	for (AActor* member : Members)
	{
		if (member == nullptr) continue;

		if (float distance = FVector::Dist(location, member->GetActorLocation()) < closestDistance)
		{
			closestMember = member;
			closestDistance = distance;
		}
	}
	return closestMember;
}

void ATeam::AddMember(AActor* member)
{
	Members.Add(member);
}

void ATeam::RemoveMember(AActor* member)
{
	if (member != nullptr)
	{
		TArray<AActor*> memberCopy = Members;
		if (memberCopy.Contains(member))
		{
			memberCopy.Remove(member);
			Members = memberCopy;
		}
	}
}

void ATeam::SetObjective(AActor* actor)
{
	CurrentObjective = actor;
}

// Called when the game starts or when spawned
void ATeam::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

