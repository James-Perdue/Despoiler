// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "SquadBlackboardComponent.h"
#include "Despoiler/TP_PlanningAgent.h"
#include "Squad.generated.h"

UCLASS()
class DESPOILER_API ASquad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASquad();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Planner)
	class USquadBlackboardComponent* SquadBlackboard;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Planner)
	class UTP_PlanningAgent* PlanningAgent;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	class UFloatingPawnMovement* MovementComponent;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Squad)
	float SpawnRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float DefaultElevation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float DefaultSpeed;

	UFUNCTION(BlueprintCallable, Category = Planner)
	EActionStatus Target();

	UFUNCTION(BlueprintCallable, Category = Planner)
	EActionStatus MoveTo(float DeltaTime);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Squad)
	TMap<TSubclassOf<class AGoalAICharacter>, int> SquadInitMembers;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnWave();

	void InitSquad();

private:

	AActor* ActiveTarget;
	FVector MoveOrigin;
	float MoveDeltaTime = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
