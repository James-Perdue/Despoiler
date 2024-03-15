// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_CharacterBlackboard.h"
#include "GoalAICharacter.h"

// Sets default values for this component's properties
UTP_CharacterBlackboard::UTP_CharacterBlackboard()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UTP_CharacterBlackboard::SetCommonLocalState(TMap<FString, bool>* LocalState)
{
	// General reset states (i.e. repeatable goals)
	LocalState->Emplace("HitTarget", false);
	LocalState->Emplace("Blocking", false);

	//LocalState->Emplace("TargetingObjective", false);
	// Target-specific goal states
	// Check if target is best target here, causing a retargeting
	/*if (CurrentTarget != nullptr && CurrentTarget->IsValidLowLevel() && !CurrentTarget->IsPendingKill())
	{
		bool atTarget = false;
		if (FVector::Distance(this->GetOwner()->GetActorLocation(), CurrentTarget->GetActorLocation()) <= MinDistance)
		{
			atTarget = true;
		}

		LocalState->Emplace("AtTarget", atTarget);
	}
	else
	{*/
		LocalState->Emplace("HasTarget", false);
		LocalState->Emplace("AtTarget", false);
		LocalState->Emplace("InFormation", false);

	//}

}

void UTP_CharacterBlackboard::UpdateFormationPosition()
{
	if (Squad == nullptr || Squad->SquadBlackboard == nullptr)
	{
		return;
	}
	
	AGoalAICharacter* owner = Cast<AGoalAICharacter>(this->GetOwner());
	FormationPosition = Squad->SquadBlackboard->FetchFormationLocation(owner);
}

TArray<AActor*> UTP_CharacterBlackboard::GetAttackingThreats()
{
	TArray<AActor*> list = TArray<AActor*>();
	for (AActor* member : OpposingTeam->GetMembers())
	{
		if (member == nullptr) continue;
		if (UTP_CharacterBlackboard* blackboard = member->GetComponentByClass<UTP_CharacterBlackboard>()) {
			if (blackboard->CurrentTarget == this->GetOwner() && blackboard->AgentState == EAgentState::Attacking)
			{
				list.Add(member);
			}
		}
	}

	return list;
}

// Called when the game starts
void UTP_CharacterBlackboard::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
//void UTP_CharacterBlackboard::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

