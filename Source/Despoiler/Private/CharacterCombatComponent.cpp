// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterCombatComponent.h"
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UCharacterCombatComponent::UCharacterCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UCharacterCombatComponent::EquipWeapon(UWeaponComponent* NewWeapon)
{
	this->Weapon = NewWeapon;

	AttackDamage = NewWeapon->Damage;

	AttackRange = NewWeapon->Range;

	AttackWarmupTime = NewWeapon->Warmup;

	AttackCooldownTime = NewWeapon->Cooldown;
}

// Called when the game starts
void UCharacterCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	AttackState = ECombatState::Idle;
	if (UTP_CharacterBlackboard* blackboard = this->GetOwner()->FindComponentByClass<UTP_CharacterBlackboard>())
	{
		CharacterBlackboard = blackboard;
	}
}


// Called every frame
void UCharacterCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

EActionStatus UCharacterCombatComponent::TryAttack(AActor* target)
{
	if (target == nullptr)
	{
		return EActionStatus::Failed;
	}

	UWorld* world = this->GetOwner()->GetWorld();
	switch (AttackState)
	{
		case ECombatState::Idle:
			if (!world->GetTimerManager().IsTimerActive(AttackWarmupTimer))
			{
				float timeWithRange = AttackWarmupTime + FMath::RandRange(-1 * AttackTimeRange, AttackTimeRange);
				world->GetTimerManager().SetTimer(AttackWarmupTimer, [this]() { EndWarmup(&AttackState); }, timeWithRange, false);
				warmupTarget = target;
			}
			break;
		case ECombatState::Ready:
		{
			//If target has changed between warmup and swing, reset attack state
			if (target != warmupTarget) {
				AttackState = ECombatState::Idle;
				break;
			}
			
			if (CharacterBlackboard->AgentState != EAgentState::Attacking)
			{
				localTarget = target;
				CharacterBlackboard->SetAgentState(EAgentState::Attacking);
			}

			if (Weapon->AttackMontage && AnimInstance && !AnimInstance->Montage_IsPlaying(nullptr))
			{
				AnimInstance->Montage_Play(Weapon->AttackMontage);
			}

			return EActionStatus::Running;
		}
		case ECombatState::Cooldown:
			return EActionStatus::Completed;
		default:
			return EActionStatus::Failed;
	}

	return EActionStatus::Running;
}

EActionStatus UCharacterCombatComponent::TryDefend(AActor* target)
{
	if (target == nullptr)
	{
		return EActionStatus::Failed;
	}

	switch (DefendState)
	{
		case ECombatState::Cooldown:
			return EActionStatus::Failed;
		default:
			CharacterBlackboard->SetAgentState(EAgentState::Defending);
	}

	if (Weapon->BlockMontage && AnimInstance && !AnimInstance->Montage_IsPlaying(nullptr))
	{
		AnimInstance->Montage_Play(Weapon->BlockMontage);
	}

	return EActionStatus::Running;
}

void UCharacterCombatComponent::HitTarget()
{
	if (localTarget == nullptr)
	{
		CharacterBlackboard->SetAgentState(EAgentState::Idle);
		return;
	}

	UWorld* world = this->GetOwner()->GetWorld();
	if (IsValid(this->GetOwner()) && IsValid(localTarget))
	{
		float distanceToTarget = FVector::Dist(this->GetOwner()->GetActorLocation(), localTarget->GetActorLocation());
		//UE_LOG(LogTemp, Log, TEXT("Distance to Target: %.2f"), distanceToTarget);
		if (distanceToTarget <= AttackRange)
		{
			//Hit Target
			FDamageInfo damageInfo = FDamageInfo();
			damageInfo.Damage = AttackDamage;
			damageInfo.DamagingActor = this->GetOwner();
			bool blocked = false;
			if (localTarget == nullptr)
			{
				return;
			}
			if (UTP_CharacterBlackboard* blackboard = localTarget->FindComponentByClass<UTP_CharacterBlackboard>())
			{
				if (blackboard->AgentState == EAgentState::Defending)
				{
					UE_LOG(LogTemp, Log, TEXT("Blocked!"));
					blocked = true;
					if (this->Weapon != nullptr && this->Weapon->BlockSound != nullptr)
					{
						UGameplayStatics::PlaySoundAtLocation(this, this->Weapon->BlockSound, this->GetOwner()->GetActorLocation());
					}
				}
			}
			if (!blocked)
			{
				if (IDamageable* damageTarget = Cast<IDamageable>(localTarget)) {
					//UE_LOG(LogTemp, Log, TEXT("Hit!"));
					damageTarget->Execute_DamageEntity(localTarget, damageInfo);
				}
				if (this->Weapon != nullptr && this->Weapon->HitSound != nullptr)
				{
					UGameplayStatics::PlaySoundAtLocation(this, this->Weapon->HitSound, this->GetOwner()->GetActorLocation());
				}
			}
		}
	}

	if (!world->GetTimerManager().IsTimerActive(AttackCooldownTimer))
	{
		world->GetTimerManager().SetTimer(AttackCooldownTimer, [this]() { EndCooldown(&AttackState); }, AttackCooldownTime, false);
	}

	CharacterBlackboard->SetAgentState(EAgentState::Idle);
	AttackState = ECombatState::Cooldown;
}

void UCharacterCombatComponent::Defend()
{
	if (!this->GetWorld()->GetTimerManager().IsTimerActive(DefendCooldownTimer))
	{
		this->GetWorld()->GetTimerManager().SetTimer(DefendCooldownTimer, [this]() { EndCooldown(&DefendState); }, DefendCooldownTime, false);
	}
	CharacterBlackboard->SetAgentState(EAgentState::Idle);
	DefendState = ECombatState::Cooldown;
}

