// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <TP_CharacterBlackboard.h>
#include "GameFramework/Character.h"
#include "GoalAICharacter.generated.h"

UCLASS(ClassGroup = (Custom))
class DESPOILER_API AGoalAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGoalAICharacter();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Planner")
	UTP_CharacterBlackboard* Blackboard;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
