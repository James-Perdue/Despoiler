// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadBlackboardComponent.h"
#include "Squad.h"

// Sets default values for this component's properties
USquadBlackboardComponent::USquadBlackboardComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void USquadBlackboardComponent::SetCommonLocalState(TMap<FString, bool>* LocalState)
{
	LocalState->Emplace("HasTarget", false);
	LocalState->Emplace("AtTarget", false);
	LocalState->Emplace("HasMission", false);
}

void USquadBlackboardComponent::RemoveMember(AGoalAICharacter* member)
{
	if (member != nullptr)
	{
		TArray<AGoalAICharacter*> memberCopy = Members;
		if (memberCopy.Contains(member))
		{
			memberCopy.Remove(member);
			Members = memberCopy;
		}
	}
}

// Called when the game starts
void USquadBlackboardComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void USquadBlackboardComponent::AssignTeam()
{
	ADespoilerGameMode* mode = Cast<ADespoilerGameMode>(this->GetOwner()->GetWorld()->GetAuthGameMode());
	if (mode->Teams[ETeam::Attacker] == nullptr || mode->Teams[ETeam::Defender] == nullptr)
	{
		return;
	}

	if (TeamAssignment == ETeam::Attacker)
	{
		mode->Teams[ETeam::Attacker]->AddSquad(Cast<ASquad>(this->GetOwner()));
		MyTeam = mode->Teams[ETeam::Attacker];
		OpposingTeam = mode->Teams[ETeam::Defender];
	}
	else
	{
		mode->Teams[ETeam::Defender]->AddSquad(Cast<ASquad>(this->GetOwner()));
		MyTeam = mode->Teams[ETeam::Defender];
		OpposingTeam = mode->Teams[ETeam::Attacker];
	}

	this->GetOwner()->GetWorld()->GetTimerManager().ClearTimer(AssignTeamTimer);
}


// Called every frame
//void USquadBlackboardComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

