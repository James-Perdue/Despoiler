// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalAICharacter.h"

// Sets default values
AGoalAICharacter::AGoalAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGoalAICharacter::BeginPlay()
{
	Super::BeginPlay();
	Blackboard = this->FindComponentByClass<UTP_CharacterBlackboard>();
	CombatComponent = this->FindComponentByClass<UCharacterCombatComponent>();
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
	if (Health <= 0)
	{
		Execute_Die(this);
	}
}

void AGoalAICharacter::Die_Implementation()
{
	Blackboard->MyTeam->RemoveMember(this);

	this->Destroy();
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

	EActionStatus combatResult = CombatComponent->TryAttack(target);
	return combatResult;
}

