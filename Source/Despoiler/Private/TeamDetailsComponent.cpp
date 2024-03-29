// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamDetailsComponent.h"

// Sets default values for this component's properties
UTeamDetailsComponent::UTeamDetailsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UTeamDetailsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//this->GetOwner()->GetWorld()->GetTimerManager().SetTimer(AssignTeamTimer, this, &UTeamDetailsComponent::AssignTeam, 0.1, true);
	ADespoilerGameMode* mode = Cast<ADespoilerGameMode>(this->GetWorld()->GetAuthGameMode());
	if (mode != nullptr)
	{
		mode->GameInitDelegate.AddDynamic(this, &UTeamDetailsComponent::AssignTeam);
	}
}

void UTeamDetailsComponent::AssignTeam()
{
	ADespoilerGameMode* mode = Cast<ADespoilerGameMode>(this->GetOwner()->GetWorld()->GetAuthGameMode());
	if (mode->Teams[ETeam::Attacker] == nullptr || mode->Teams[ETeam::Defender] == nullptr)
	{
		return;
	}

	if (TeamAssignment == ETeam::Attacker)
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

	//this->GetOwner()->GetWorld()->GetTimerManager().ClearTimer(AssignTeamTimer);
}

// Called every frame
//void UTeamDetailsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

