// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <GeneralEnums.h>
#include <IDamageable.h>
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

protected:

	FTimerHandle AttackWarmupTimer;
	FTimerHandle AttackCooldownTimer;

	EAttackState attackState;

	// Called when the game starts
	virtual void BeginPlay() override;
	void EndCooldown() { attackState = EAttackState::Idle; }
	void EndWarmup() { attackState = EAttackState::Ready; }

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	EActionStatus TryAttack(AActor* target);
};
