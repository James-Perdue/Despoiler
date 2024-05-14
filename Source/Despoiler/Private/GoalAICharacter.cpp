// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalAICharacter.h"
#include <Kismet/KismetMathLibrary.h>
#include "Logging/StructuredLog.h"
#include <AIController.h>

// Sets default values
AGoalAICharacter::AGoalAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PlanningAgent = CreateDefaultSubobject<UTP_PlanningAgent>(TEXT("PlanningAgent"));
	Blackboard = CreateDefaultSubobject<UTP_CharacterBlackboard>(TEXT("Blackboard"));
	Blackboard->CurrentTarget = nullptr;

	CombatComponent = CreateDefaultSubobject<UCharacterCombatComponent>(TEXT("CombatComponent"));
	InteractComponent = CreateDefaultSubobject<UCharacterInteractComponent>(TEXT("InteractComponent"));

	//Weapon = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon"));

}

// Called when the game starts or when spawned
void AGoalAICharacter::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	
	Weapon = NewObject<UWeaponComponent>(this, WeaponClass);
	if (Weapon != nullptr)
	{
		Weapon->RegisterComponent();
		Weapon->AttachToComponent(this->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RHandSocket"));
	}
	CombatComponent->EquipWeapon(Weapon);
	CombatComponent->AnimInstance = this->GetMesh()->GetAnimInstance();
}

// Called every frame
void AGoalAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGoalAICharacter::DamageEntity_Implementation(FDamageInfo DamageInfo)
{
	Health -= DamageInfo.Damage;
	if (Health <= 0 && Alive)
	{
		Execute_Die(this);
	}
}

void AGoalAICharacter::Die_Implementation()
{
	//TODO: Change these to be delegates on team and squad respectively
	Alive = false;

	Blackboard->MyTeam->RemoveMember(this);
	Blackboard->Squad->SquadBlackboard->RemoveMember(this);
	if (PlanningAgent != nullptr)
	{
		PlanningAgent->StopPlanning();
	}

	ADespoilerGameMode* mode = Cast<ADespoilerGameMode>(this->GetWorld()->GetAuthGameMode());
	if (mode != nullptr)
	{
		mode->DeathDelegate.Broadcast(this);
	}

	this->GetMesh()->SetCollisionProfileName("Ragdoll");
	this->GetMesh()->SetSimulatePhysics(true);
	this->SetLifeSpan(4);
}

EActionStatus AGoalAICharacter::AttackTarget_Implementation()
{
	if (CombatComponent == nullptr || Blackboard == nullptr)
	{
		return EActionStatus::Failed;
	}
	AActor* target = Blackboard->CurrentTarget;
	if (target == nullptr)
	{
		return EActionStatus::Failed;
	}

	FVector TargetLocation = target->GetActorLocation();
	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), TargetLocation);
	this->SetActorRotation(FRotator(this->GetActorRotation().Pitch, this->GetActorRotation().Yaw, NewRotation.Roll));

	EActionStatus attackResult = CombatComponent->TryAttack(target);
	return attackResult;
}

EActionStatus AGoalAICharacter::Defend_Implementation()
{
	if (CombatComponent == nullptr || Blackboard == nullptr)
	{
		return EActionStatus::Failed;
	}
	AActor* target = Blackboard->CurrentTarget;
	if (target == nullptr)
	{
		return EActionStatus::Failed;
	}

	EActionStatus blockResult = CombatComponent->TryDefend(target);
	return blockResult;
}

EActionStatus AGoalAICharacter::Target_Implementation()
{
	if (CombatComponent == nullptr || Blackboard == nullptr)
	{
		return EActionStatus::Failed;
	}
	AActor* target = nullptr;

	if (Blackboard->Squad == nullptr || Blackboard->Squad->SquadBlackboard == nullptr)
	{
		return EActionStatus::Failed;
	}

	switch (Blackboard->Squad->SquadBlackboard->SquadState)
	{
		case ESquadState::Capture:
		{
			if (Blackboard->MyTeam == nullptr)
			{
				return EActionStatus::Failed;
			}

			if (AActor* objective = Blackboard->MyTeam->GetObjective())
			{
				target = objective;
			}
			break;
		}
		case ESquadState::Attack:
		case ESquadState::HoldPosition:
		case ESquadState::Free:
		{
			if (Blackboard->OpposingTeam == nullptr)
			{
				return EActionStatus::Failed;
			}

			TArray<AActor*> attackers = Blackboard->GetAttackingThreats();
			if (attackers.Num() > 0)
			{
				target = UGeneralUtil::GetClosestActor(this->GetActorLocation(), attackers);
			}
			else
			{
				target = Blackboard->OpposingTeam->GetClosestMember(this->GetActorLocation());
			}
			break;
		}
	}

	if (target == nullptr)
	{
		return EActionStatus::Failed;
	}

	Blackboard->CurrentTarget = target;
	return EActionStatus::Completed;
}

EActionStatus AGoalAICharacter::MoveTo_Implementation(FVector targetLocation, float minDistance, bool bStopOnOverlap)
{
	if (Blackboard == nullptr || GetController() == nullptr)
	{
		return EActionStatus::Failed;
	}
	AAIController* controller = Cast<AAIController>(GetController());
	if (controller == nullptr)
	{
		return EActionStatus::Failed;
	}
	//Still too far
	double dist = FVector::Dist(this->GetActorLocation(), targetLocation);
	//UE_LOGFMT(LogTemp, Log, "Distance to goal: {1}, minDistance {2}. Final Location: {3}", dist, minDistance, targetLocation.ToString());

	if (!FMath::IsNearlyEqual(dist, minDistance, .0005))
	{
		EPathFollowingRequestResult::Type result = controller->MoveToLocation(targetLocation, minDistance, bStopOnOverlap);

		if (result == 0)
		{
			//Failed
			return EActionStatus::Failed;
		}
		if (result == 2)
		{
			return EActionStatus::Running;
		}
	}

	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), targetLocation);
	this->SetActorRotation(FRotator(this->GetActorRotation().Pitch, this->GetActorRotation().Yaw, NewRotation.Roll));

	return EActionStatus::Completed;
}

EActionStatus AGoalAICharacter::Capture_Implementation()
{
	if (InteractComponent == nullptr)
	{
		return EActionStatus::Failed;
	}

	return InteractComponent->TryCapture();
}

