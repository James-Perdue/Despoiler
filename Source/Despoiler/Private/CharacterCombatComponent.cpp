// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterCombatComponent.h"

// Sets default values for this component's properties
UCharacterCombatComponent::UCharacterCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCharacterCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	attackState = EAttackState::Idle;
}


// Called every frame
void UCharacterCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

EActionStatus UCharacterCombatComponent::TryAttack(AActor* target)
{
	//this->GetOwner()->GetWorld()->GetTimerManager().SetTimer(AssignTeamTimer, this, &UTP_TeamDetails::AssignTeam, 0.1, true);
	UWorld* world = this->GetOwner()->GetWorld();
	switch (attackState)
	{
		case EAttackState::Idle:
			if (!world->GetTimerManager().IsTimerActive(AttackWarmupTimer))
			{
				world->GetTimerManager().SetTimer(AttackWarmupTimer, this, &UCharacterCombatComponent::EndWarmup, AttackWarmupTime, false);
			}
			break;
		case EAttackState::Ready:
		{
			float distanceToTarget = FVector::Dist(this->GetOwner()->GetActorLocation(), target->GetActorLocation());
			UE_LOG(LogTemp, Log, TEXT("Distance to Target: %.2f"), distanceToTarget);
			//Attack
			if (distanceToTarget <= AttackRange)
			{
				//Hit Target
				FDamageInfo damageInfo = FDamageInfo();
				damageInfo.Damage = AttackDamage;
				damageInfo.DamagingActor = this->GetOwner();

				IDamageable* damageTarget = Cast<IDamageable>(target);
				damageTarget->Execute_DamageEntity(target, damageInfo);
			}
			if (!world->GetTimerManager().IsTimerActive(AttackCooldownTimer))
			{
				world->GetTimerManager().SetTimer(AttackCooldownTimer, this, &UCharacterCombatComponent::EndCooldown, AttackCooldownTime, false);
			}
			attackState = EAttackState::Cooldown;
			return EActionStatus::Completed;
		}
		case EAttackState::Cooldown:
			break;
		default:
			return EActionStatus::Failed;
	}

	return EActionStatus::Running;
}

