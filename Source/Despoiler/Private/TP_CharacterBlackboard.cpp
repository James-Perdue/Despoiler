// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_CharacterBlackboard.h"

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
	if (CurrentTarget != nullptr)
	{
		bool atTarget = false;
		if (FVector::Distance(this->GetOwner()->GetActorLocation(), CurrentTarget->GetActorLocation()) <= MinDistance)
		{
			atTarget = true;
		}
		/*if (*LocalState->Find("AtTarget") == false) 
		{
			if (FVector::Distance(this->GetOwner()->GetActorLocation(), CurrentTarget->GetActorLocation()) <= MinDistance)
			{
				LocalState->Emplace("AtTarget", true);
			}
		}*/
		LocalState->Emplace("AtTarget", atTarget);
	}
	else
	{
		LocalState->Emplace("HasTarget", false);
		LocalState->Emplace("AtTarget", false);
	}
}

// Called when the game starts
void UTP_CharacterBlackboard::BeginPlay()
{
	Super::BeginPlay();

	// ...
	ADespoilerGameMode* mode = Cast<ADespoilerGameMode>(this->GetOwner()->GetWorld()->GetAuthGameMode());
	if(TeamAssignment == ETeam::Attacker)
	{
		mode->Teams[ETeam::Attacker]->AddMember(this->GetOwner());
		MyTeam = mode->Teams[ETeam::Attacker];
		OpposingTeam = mode->Teams[ETeam::Defender];

	}
	else 
	{
		mode->Teams[ETeam::Defender]->AddMember(this->GetOwner());
		MyTeam = mode->Teams[ETeam::Defender];
		OpposingTeam = mode->Teams[ETeam::Attacker];

	}
}


// Called every frame
void UTP_CharacterBlackboard::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

