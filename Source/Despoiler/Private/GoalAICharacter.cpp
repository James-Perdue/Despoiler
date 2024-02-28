// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalAICharacter.h"
#include <Kismet/KismetMathLibrary.h>

// Sets default values
AGoalAICharacter::AGoalAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PlanningAgent = CreateDefaultSubobject<UTP_PlanningAgent>(TEXT("PlanningAgent"));
	Blackboard = CreateDefaultSubobject<UTP_CharacterBlackboard>(TEXT("Blackboard"));
	Blackboard->CurrentTarget = nullptr;

	CombatComponent = CreateDefaultSubobject<UCharacterCombatComponent>(TEXT("CombatComponent"));

}

// Called when the game starts or when spawned
void AGoalAICharacter::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
}

// Called every frame
void AGoalAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGoalAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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
	Blackboard->MyTeam->RemoveMember(this);
	Blackboard->Squad->SquadBlackboard->RemoveMember(this);
	if (PlanningAgent != nullptr)
	{
		PlanningAgent->StopPlanning();
	}
	Alive = false;
	this->GetMesh()->SetCollisionProfileName("Ragdoll");
	this->GetMesh()->SetSimulatePhysics(true);
	this->SetLifeSpan(2);
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

	TArray<AActor*> attackers = Blackboard->GetAttackingThreats();
	if (attackers.Num() > 0)
	{
		target = UGeneralUtil::GetClosestActor(this->GetActorLocation(), attackers);
	}
	else
	{
		target = Blackboard->OpposingTeam->GetClosestMember(this->GetActorLocation());
	}

	if (target == nullptr)
	{
		return EActionStatus::Failed;
	}

	Blackboard->CurrentTarget = target;
	return EActionStatus::Completed;
}

