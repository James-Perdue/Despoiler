// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <TP_CharacterBlackboard.h>
#include "GameFramework/Character.h"
#include "IDamageable.h"
#include <CharacterCombatComponent.h>
#include <GeneralCharacter.h>
#include <WeaponComponent.h>
#include "GoalAICharacter.generated.h"

UCLASS(ClassGroup = (Custom))
class DESPOILER_API AGoalAICharacter : public ACharacter, public IDamageable, public IGeneralCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGoalAICharacter();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Planner)
	class UTP_CharacterBlackboard* Blackboard;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Planner)
	class UCharacterCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Planner)
	class UTP_PlanningAgent* PlanningAgent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UWeaponComponent> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UWeaponComponent* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	int MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	int Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	bool Alive = true;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void DamageEntity_Implementation(FDamageInfo DamageInfo) override;

	virtual void Die_Implementation() override;

	virtual EActionStatus  AttackTarget_Implementation() override;

	virtual EActionStatus  Defend_Implementation() override;

	virtual EActionStatus  Target_Implementation() override;

	virtual EActionStatus  MoveTo_Implementation(FVector targetLocation, float minDistance, bool bStopOnOverlap) override;





protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
