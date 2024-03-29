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
			UpdateFormation();
		}
	}
	if (Members.Num() == 0)
	{
		this->GetOwner()->Destroy();
	}
}

void USquadBlackboardComponent::SetFormation()
{
	//Size of Matrix is num rows * num columns
	FormationInfo.NumRows = FMath::CeilToInt((float)Members.Num() / FormationInfo.FormationWidth);
	FormationInfo.NumColumns = FMath::CeilToInt((float)Members.Num() / FormationInfo.NumRows);

	UpdateLeader();
	for (int i = 0; i < FormationInfo.NumColumns; i++)
	{
		Columns.Add(new FColumn);
	}
	//FormationMatrix.SetNum(FormationInfo.NumRows * FormationInfo.NumColumns, false);

	//For each row,
	for (int row = 0; row < FormationInfo.NumRows; row++)
	{
		//Form a column
		for (int col = 0; col < FormationInfo.NumColumns; col++)
		{
			int formationIndex = row * FormationInfo.FormationWidth + col;
			if (Members.IsValidIndex(formationIndex))
			{
				AGoalAICharacter* member = Members[formationIndex];
				if (member == nullptr || member->Alive == false)
				{
					continue;
				}

				Columns[col]->Members.Emplace(member);
				//FormationMatrix[formationIndex] = member;

				//Ignore leader
				/*if (row == 0 && col == 0)
				{
					continue;
				}*/

				RepositionMember(row, col);
			}
		}
	}
}

void USquadBlackboardComponent::UpdateFormation()
{
	//Set conditions for reformation
	if (Members.Num() < .25f * FormationInfo.NumColumns * FormationInfo.NumRows)
	{
		SetFormation();
		UE_LOGFMT(LogTemp, Log, "Resetting Formation");
		return;
	}

	UpdateLeader();

	for (int row = 0; row < FormationInfo.NumRows; row++)
	{
		for (int col = 0; col < Columns.Num(); col++)
		{
			if (!Columns[col]->Members.IsValidIndex(row) || (Columns[col]->Members[row] != nullptr && Columns[col]->Members[row]->Alive))
			{
				//Member is fine or invalid index
				continue;
			}

			//Replacement unit needed, check same column first
			if (!Columns[col]->Members.IsValidIndex(row + 1))
			{
				continue;
			}

			if (Columns[col]->Members[row + 1] == nullptr || !Columns[col]->Members[row + 1]->Alive)
			{
				//Same column invalid, maybe only do this for first units?
				int left = col - 1;
				int right = col + 1;
				while (left > 0 || right < FormationInfo.NumColumns)
				{
					bool foundReplacement = false;
					//&& Columns[left]->Members.Num() > 1 && Columns[left]->Members.Num() > Columns[col]->Members.Num() + 1
					if (Columns.IsValidIndex(left))
					{
						for (int i = Columns[left]->Members.Num() - 1; i > row; i--)
						{
							if (Columns[left]->Members[i] != nullptr && Columns[left]->Members[i]->Alive)
							{
								Columns[col]->Members[row] = Columns[left]->Members[i];
								Columns[left]->Members[i] = nullptr;
								RepositionMember(row, col);
								foundReplacement = true;
								break;
							}
						}
					}
					if (Columns.IsValidIndex(right))
					{
						for (int j = Columns[right]->Members.Num() - 1; j > row; j--)
						{
							if (Columns[right]->Members[j] != nullptr && Columns[right]->Members[j]->Alive)
							{
								Columns[col]->Members[row] = Columns[right]->Members[j];
								Columns[right]->Members[j] = nullptr;
								RepositionMember(row, col);
								foundReplacement = true;
								break;
							}
						}
					}
					if (foundReplacement)
					{
						break;
					}
					left--; 
					right++;
				}
				continue;
			}

			Columns[col]->Members[row] = Columns[col]->Members[row + 1];

			RepositionMember(row, col);

			Columns[col]->Members[row + 1] = nullptr;
			continue;
		}
	}
}

FVector USquadBlackboardComponent::FetchFormationLocation(AGoalAICharacter* member)
{
	if (FormationInfo.Leader == nullptr || member == nullptr)
	{
		return FVector(0, 0, 0);
	}

	FVector* result = FormationMap.Find(member);
	if (result != nullptr)
	{
		//int maxInRow = FMath::FloorToInt((float)Members.Num() / FormationInfo.NumRows);
		int adjustedRowWidth = FormationInfo.NumColumns > Members.Num() ? Members.Num() : FormationInfo.NumColumns;
		//if (adjustedRowWidth != FormationInfo.NumColumns)
		//{
		//	//UE_LOGFMT(LogTemp, Log, "AdjustedWidth {1}", adjustedRowWidth);
		//}

		FVector squadLocation = FVector(this->GetOwner()->GetActorLocation().X, this->GetOwner()->GetActorLocation().Y, member->GetActorLocation().Z);
		FVector formationOrigin = FVector(squadLocation.X, squadLocation.Y - 0.5f * adjustedRowWidth * FormationInfo.FormationSpacing, squadLocation.Z);

		FVector rotatedOrigin = UGeneralUtil::RotateVector(squadLocation, formationOrigin, this->GetOwner()->GetActorRotation());

		FVector positionWithOffset = rotatedOrigin + *result;
		FVector rotatedVector = UGeneralUtil::RotateVector(rotatedOrigin, positionWithOffset, this->GetOwner()->GetActorRotation());
		
		return rotatedVector;
		
	}
	return FVector(0,0,0);
}

// Called when the game starts
void USquadBlackboardComponent::BeginPlay()
{
	Super::BeginPlay();
	//GetWorld()->GetTimerManager().SetTimer(UpdateFormationTimer, this, &USquadBlackboardComponent::SetFormation, .05, true);
	/*ADespoilerGameMode* mode = Cast<ADespoilerGameMode>(this->GetWorld()->GetAuthGameMode());
	if (mode != nullptr)
	{
		mode->GameInitDelegate.AddDynamic(this, &AssignTeam);
	}*/
	
	//Size of Matrix is num rows * num columns

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

	//this->GetOwner()->GetWorld()->GetTimerManager().ClearTimer(AssignTeamTimer);
}

void USquadBlackboardComponent::UpdateLeader()
{
	// ADjusted row width helps formation condense down
	int adjustedRowWidth = FormationInfo.NumColumns > Members.Num() ? Members.Num() : FormationInfo.NumColumns;
	//Trace to ground
	if (FormationInfo.Leader == nullptr || !FormationInfo.Leader->Alive)
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
		float yOffset = FormationInfo.NumColumns % 2 == 0 ? 0.5f * adjustedRowWidth * FormationInfo.FormationSpacing : FMath::Floor((float)adjustedRowWidth / 2) * FormationInfo.FormationSpacing;
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

		FormationInfo.Leader = Cast<AGoalAICharacter>(leaderActor);
		FormationMap.Emplace(FormationInfo.Leader, FVector(0, 0, 0));
	}
}

void USquadBlackboardComponent::RepositionMember(int row, int col)
{
	FVector placementOffset = FVector(0, 0, 0);
	placementOffset.X = -1 * row * FormationInfo.FormationSpacing;
	placementOffset.Y = col * FormationInfo.FormationSpacing;
	FormationMap.Emplace(Columns[col]->Members[row], placementOffset);
}



// Called every frame
//void USquadBlackboardComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

