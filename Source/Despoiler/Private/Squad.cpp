// Fill out your copyright notice in the Description page of Project Settings.


#include "Squad.h"
#include "GoalAICharacter.h"
#include <AIController.h>
#include <Kismet/KismetMathLibrary.h>

// Sets default values
ASquad::ASquad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SquadBlackboard = CreateDefaultSubobject<USquadBlackboardComponent>(TEXT("SquadBlackboard"));
	SquadBlackboard->SquadState = ESquadState::Idle;
	SquadBlackboard->CurrentTarget = nullptr;

	PlanningAgent = CreateDefaultSubobject<UTP_PlanningAgent>(TEXT("PlanningAgent"));

}

EActionStatus ASquad::Target()
{
	if (SquadBlackboard == nullptr)
	{
		return EActionStatus::Failed;
	}

	switch (SquadBlackboard->SquadState)
	{
	case ESquadState::Capture:
	{
		if (SquadBlackboard->MyTeam == nullptr)
		{
			return EActionStatus::Failed;
		}

		if (AActor* objective = SquadBlackboard->MyTeam->GetObjective())
		{
			SquadBlackboard->CurrentTarget = objective;
		}
		break;
	}
		

	case ESquadState::Attack:
	{
		if (SquadBlackboard->OpposingTeam == nullptr)
		{
			return EActionStatus::Failed;
		}
		TArray<ASquad*> squads = SquadBlackboard->OpposingTeam->GetSquads();
		if (squads.Num() > 0)
		{
			TArray<AActor*> squadActors = TArray<AActor*>();
			for (ASquad* squad : squads)
			{
				if (squad->SquadBlackboard->Members.Num() > 0)
				{
					squadActors.Add(Cast<AActor>(squad));
				}
			}
			SquadBlackboard->CurrentTarget = UGeneralUtil::GetClosestActor(this->GetActorLocation(), squadActors);
		}
		break;
	}
	case ESquadState::HoldPosition:
	{
		if (SquadBlackboard->MyTeam == nullptr)
		{
			return EActionStatus::Failed;
		}

		TArray<APositionMarker*> positions = SquadBlackboard->MyTeam->GetPositions();
		if (positions.Num() > 0)
		{
			TArray<AActor*> copiedActors = TArray<AActor*>();
			for (APositionMarker* position : positions)
			{
				copiedActors.Add(Cast<AActor>(position));
			}
			SquadBlackboard->CurrentTarget = UGeneralUtil::GetClosestActor(this->GetActorLocation(), copiedActors);
		}
		break;
	}
	default:
		break;
	}

	return EActionStatus::Completed;
}

EActionStatus ASquad::MoveTo(float DeltaTime)
{
	if (SquadBlackboard == nullptr || SquadBlackboard->CurrentTarget == nullptr)
	{
		return EActionStatus::Failed;
	}
	AActor* target = SquadBlackboard->CurrentTarget;
	if (target != ActiveTarget) 
	{
		ActiveTarget = target;
		MoveOrigin = this->GetActorLocation();
		MoveDeltaTime = 0;
	}
	FVector targetLocation = target->GetActorLocation();
	//TODO: Add logic to use raycasting to have squad height be dynamic
	targetLocation.Z = DefaultElevation;

	float distanceToTarget = FVector::Dist(this->GetActorLocation(), targetLocation);
	if (distanceToTarget < 1)
	{
		return EActionStatus::Completed;
	}
	MoveDeltaTime += DeltaTime;

	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), targetLocation);
	this->SetActorRotation(NewRotation);

	FVector NewLocation = FMath::VInterpConstantTo(this->GetActorLocation(), targetLocation, MoveDeltaTime, DefaultSpeed);

	this->SetActorLocation(NewLocation);
	/*AAIController* controller = Cast<AAIController>(this->GetController());
	EPathFollowingRequestResult::Type result = controller->MoveToLocation(targetLocation);
	if(result == 0) return EActionStatus::Failed;
	if (result == 1) return EActionStatus::Completed;*/
	
	return EActionStatus::Running;
}

// Called when the game starts or when spawned
void ASquad::BeginPlay()
{
	Super::BeginPlay();
	//MovementComponent->MaxSpeed = DefaultSpeed;
	InitSquad();
	FVector initLocation = FVector(this->GetActorLocation().X, this->GetActorLocation().Y, DefaultElevation);
	this->SetActorLocation(initLocation);
}

void ASquad::SpawnWave()
{
	for (const auto& SpawnPair : SquadInitMembers) {
		for (int i = 0; i < SpawnPair.Value; i++) {
			AGoalAICharacter* spawnedMember = nullptr;
			while (spawnedMember == nullptr)
			{
				FVector2D RandomPoint = FMath::RandPointInCircle(SpawnRadius);
				FVector SpawnPoint = FVector(this->GetActorLocation().X + RandomPoint.X, this->GetActorLocation().Y + RandomPoint.Y, this->GetActorLocation().Z);
				UWorld* World = this->GetWorld();
				FActorSpawnParameters spawnParameters;

				spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
				spawnedMember = World->SpawnActor<AGoalAICharacter>(SpawnPair.Key, SpawnPoint, FRotator::ZeroRotator);
			}
			spawnedMember->Blackboard->Squad = this;
			spawnedMember->SpawnDefaultController();
			SquadBlackboard->Members.Add(spawnedMember);
		}
	}
}

void ASquad::InitSquad()
{
	if (SquadBlackboard == nullptr)
	{
		return;
	}
	UWorld* World = this->GetWorld();

	int totalToSpawn = 0;
	for (const auto& SpawnPair : SquadInitMembers) 
	{
		totalToSpawn += SpawnPair.Value;
	}

	int adjustedRowWidth = SquadBlackboard->FormationInfo.FormationWidth > totalToSpawn ? totalToSpawn : SquadBlackboard->FormationInfo.FormationWidth;

	float yOffset = SquadBlackboard->FormationInfo.FormationWidth % 2 == 0 ? 0.5f * adjustedRowWidth * SquadBlackboard->FormationInfo.FormationSpacing : FMath::Floor((float)adjustedRowWidth / 2) * SquadBlackboard->FormationInfo.FormationSpacing;
	FVector placementLocation = FVector
	(
		this->GetActorLocation().X,
		this->GetActorLocation().Y - yOffset,
		this->GetActorLocation().Z
	);

	int currentRow = 0;
	int currentRowIndex = 0;

	for (const auto& SpawnPair : SquadInitMembers) {
		for (int i = 0; i < SpawnPair.Value; i++) {
			currentRowIndex++;

			AGoalAICharacter* spawnedMember = nullptr;
			FVector SpawnPoint = FVector(placementLocation.X, placementLocation.Y, this->GetActorLocation().Z);
			FActorSpawnParameters spawnParameters;
			FVector rotationVector = SpawnPoint - GetActorLocation();
			rotationVector = GetActorRotation().RotateVector(rotationVector);
			SpawnPoint = GetActorLocation() + rotationVector;

			spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			spawnedMember = World->SpawnActor<AGoalAICharacter>(SpawnPair.Key, SpawnPoint, GetActorRotation());
			if (spawnedMember != nullptr)
			{
				spawnedMember->Blackboard->Squad = this;
				spawnedMember->Blackboard->TeamAssignment = SquadBlackboard->TeamAssignment;
				spawnedMember->SpawnDefaultController();
				SquadBlackboard->Members.Add(spawnedMember);
				placementLocation.Y += SquadBlackboard->FormationInfo.FormationSpacing;
				if (currentRowIndex >= adjustedRowWidth)
				{
					currentRowIndex = 0;
					currentRow++;
					placementLocation.X = placementLocation.X - SquadBlackboard->FormationInfo.FormationSpacing;
					placementLocation.Y = this->GetActorLocation().Y - yOffset;

				}
			}
			
		}
	}
	SquadBlackboard->SetFormation();

}

// Called every frame
void ASquad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

