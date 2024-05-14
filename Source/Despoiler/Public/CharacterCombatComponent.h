// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <GeneralEnums.h>
#include <IDamageable.h>
#include <TP_CharacterBlackboard.h>
#include <WeaponComponent.h>
#include "CharacterCombatComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DESPOILER_API UCharacterCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterCombatComponent();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	int AttackRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	int AttackDamage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	float AttackWarmupTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	float AttackCooldownTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	float DefendCooldownTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	float AttackTimeRange;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	ECombatState AttackState;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	ECombatState DefendState;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	UWeaponComponent* Weapon;

	UPROPERTY(BlueprintReadWrite, Category = "Animation")
	UAnimInstance* AnimInstance;

	UFUNCTION(BlueprintCallable)
	void EquipWeapon(UWeaponComponent* NewWeapon);

protected:

	FTimerHandle AttackWarmupTimer;
	FTimerHandle AttackCooldownTimer;

	FTimerHandle DefendCooldownTimer;

	AActor* warmupTarget;
	AActor* localTarget;

	// Called when the game starts
	virtual void BeginPlay() override;
	void EndCooldown(ECombatState* state) { *state = ECombatState::Idle; }
	void EndWarmup(ECombatState* state) { *state = ECombatState::Ready; }

	UTP_CharacterBlackboard* CharacterBlackboard;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	EActionStatus TryAttack(AActor* target);

	EActionStatus TryDefend(AActor* target);

	UFUNCTION(BlueprintCallable, Category = "GOAP")
	void HitTarget();

	UFUNCTION(BlueprintCallable, Category = "GOAP")
	void Defend();
};
