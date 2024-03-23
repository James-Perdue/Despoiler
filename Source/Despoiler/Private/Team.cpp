// Fill out your copyright notice in the Description page of Project Settings.


#include "Team.h"
#include <Despoiler/DespoilerGameMode.h>

// Sets default values
ATeam::ATeam()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

AActor* ATeam::GetClosestMember(FVector location)
{
	return UGeneralUtil::GetClosestActor(location, Members);
}

void ATeam::AddMember(AActor* member)
{
	Members.Add(member);
	ADespoilerGameMode* mode = Cast<ADespoilerGameMode>(this->GetWorld()->GetAuthGameMode());

	if (mode != nullptr)
	{
		mode->TeamUpdateDelegate.Broadcast();
	}
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

			ADespoilerGameMode* mode = Cast<ADespoilerGameMode>(this->GetWorld()->GetAuthGameMode());

			if (mode != nullptr)
			{
				mode->TeamUpdateDelegate.Broadcast();
			}
		}
	}
}

void ATeam::AddSquad(ASquad* squad)
{
	Squads.Add(squad);
}

void ATeam::RemoveSquad(ASquad* squad)
{
	if (squad != nullptr)
	{
		TArray<ASquad*> squadCopy = Squads;
		if (squadCopy.Contains(squad))
		{
			squadCopy.Remove(squad);
			Squads = squadCopy;
		}
	}
}

void ATeam::SetObjective(AActor* actor)
{
	CurrentObjective = actor;
}

void ATeam::AddPosition(APositionMarker* member)
{
	Positions.Add(member);
}

void ATeam::RemovePosition(APositionMarker* member)
{
	if (member != nullptr)
	{
		TArray<APositionMarker*> listCopy = Positions;
		if (listCopy.Contains(member))
		{
			listCopy.Remove(member);
			Positions = listCopy;
		}
	}
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

