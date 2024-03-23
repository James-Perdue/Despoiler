// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadBlackboardComponent.h"
#include "GoalAICharacter.h"
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
			SetFormation();
		}
	}
	if (Members.Num() == 0)
	{
		this->GetOwner()->Destroy();
	}
}

void USquadBlackboardComponent::SetFormation()
{
	// ADjusted row width helps formation condense down
	int adjustedRowWidth = FormationWidth > Members.Num() ? Members.Num() : FormationWidth;
	//Trace to ground
	if (Leader == nullptr || !Leader->Alive)
	{
		FVector startLocation = this->GetOwner()->GetActorLocation();
		FVector endLocation = startLocation + FVector::DownVector * 1000;
		FHitResult hitResult;
		FVector groundLocation = FVector(0, 0, 0);
		if (GetWorld()->LineTraceSingleByChannel(hitResult, startLocation, endLocation, ECC_Visibility))
		{
			groundLocation = hitResult.ImpactPoint;
		}

		//For common formation start with upper left
		float yOffset = FormationWidth % 2 == 0 ? 0.5f * adjustedRowWidth * FormationSpacing : FMath::Floor((float)adjustedRowWidth / 2) * FormationSpacing;
		FVector placementLocation = FVector(groundLocation.X, groundLocation.Y - yOffset, groundLocation.Z);

		FVector rotationVector = placementLocation - FVector(this->GetOwner()->GetActorLocation().X, this->GetOwner()->GetActorLocation().Y, placementLocation.Z);
		rotationVector = this->GetOwner()->GetActorRotation().RotateVector(rotationVector);
		placementLocation = FVector(this->GetOwner()->GetActorLocation().X, this->GetOwner()->GetActorLocation().Y, placementLocation.Z) + rotationVector;
		TArray<AActor*> memberActors = TArray<AActor*>();
		for (AGoalAICharacter* actor : Members)
		{
			memberActors.Add(Cast<AActor>(actor));
			
		}
		AActor* leaderActor = UGeneralUtil::GetClosestActor(placementLocation, memberActors);
		if (leaderActor == nullptr) {
			UE_LOG(LogTemp, Log, TEXT("Squad failed to find a good leader"));
			return;
		}

		Leader = Cast<AGoalAICharacter>(leaderActor);
		FormationMap.Emplace(Leader, FVector(0, 0, 0));
	}
	GetWorld()->GetTimerManager().ClearTimer(UpdateFormationTimer);

	FVector placementOffset = FVector(0,0,0);

	TArray<AGoalAICharacter*> memberCopy = Members;
	memberCopy.Remove(Leader);
	/*FVector LeaderLocation = Leader->GetActorLocation();
	memberCopy.Sort([LeaderLocation] (const AGoalAICharacter& A, const AGoalAICharacter& B) {
		return FVector::DistSquared(LeaderLocation, A.GetActorLocation()) < FVector::DistSquared(LeaderLocation, B.GetActorLocation());
	});*/

	int currentRow = 0;
	int currentRowIndex = 1;
	for (AGoalAICharacter* member : memberCopy)
	{
		if (member == nullptr || member->Alive == false)
		{
			continue;
		}
		placementOffset.Y += FormationSpacing;
		if (currentRowIndex >= adjustedRowWidth)
		{
			currentRowIndex = 0;
			currentRow++;
			placementOffset.X = placementOffset.X - FormationSpacing;
			placementOffset.Y = 0;

		}

		FormationMap.Emplace(member, placementOffset);
		currentRowIndex++;
	}
}

FVector USquadBlackboardComponent::FetchFormationLocation(AGoalAICharacter* member)
{
	if (Leader == nullptr)
	{
		return FVector(0, 0, 0);
	}

	FVector* result = FormationMap.Find(member);
	if (result != nullptr)
	{
		//You are leader
		if (member == Leader)
		{
			int adjustedRowWidth = FormationWidth > Members.Num() ? Members.Num() : FormationWidth;

			FVector initialPlacement = FVector(this->GetOwner()->GetActorLocation().X, this->GetOwner()->GetActorLocation().Y - 0.5f * adjustedRowWidth * FormationSpacing, member->GetActorLocation().Z);
			FVector rotationVector = initialPlacement - FVector(this->GetOwner()->GetActorLocation().X, this->GetOwner()->GetActorLocation().Y, initialPlacement.Z);
			rotationVector = this->GetOwner()->GetActorRotation().RotateVector(rotationVector);
			return FVector(this->GetOwner()->GetActorLocation().X, this->GetOwner()->GetActorLocation().Y, initialPlacement.Z) + rotationVector;
		}
		else {
			FVector newLocation = *result;
			if (Leader != nullptr)
			{
				FVector rotationVector = *result;
				rotationVector = this->GetOwner()->GetActorRotation().RotateVector(rotationVector);
				newLocation = Leader->GetActorLocation() + rotationVector;
			}
			
			//UE_LOGFMT(LogTemp, Log, "Vector Rep: ROtator: {1} Result: {2} Rotated: {3}, Final: {4}", Leader->GetActorRotation().ToString(), result->ToString(), rotationVector.ToString(), newLocation.ToString());
			return newLocation;
		}
		
	}
	return FVector(0,0,0);
}

// Called when the game starts
void USquadBlackboardComponent::BeginPlay()
{
	Super::BeginPlay();
	//TODO: trigger this only until a squad formation is set, then when members die
	GetWorld()->GetTimerManager().SetTimer(UpdateFormationTimer, this, &USquadBlackboardComponent::SetFormation, .05, true, -1);
	//SetFormation();
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

