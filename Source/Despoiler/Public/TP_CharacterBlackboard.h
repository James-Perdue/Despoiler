// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Despoiler/DespoilerGameMode.h>
#include "GeneralEnums.h"
#include "BlackboardBaseComponent.h"
#include <Squad.h>
#include "TP_CharacterBlackboard.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DESPOILER_API UTP_CharacterBlackboard : public UBlackboardBaseComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTP_CharacterBlackboard();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Planner")
	float MinDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Planner")
	float AgroRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Planner")
	bool InCombat = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Planner")
	float CombatCheckGoalFrequency = .1f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Planner")
	EAgentState AgentState = EAgentState::Idle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Planner")
	ASquad* Squad;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Planner")
	FVector FormationPosition;

	UFUNCTION(BlueprintCallable, Category = "Planner")
	void UpdateFormationPosition();

	UFUNCTION(BlueprintCallable, Category = "Planner")
	void SetAgentState(EAgentState state) { AgentState = state; }

	// Get list of actors that are attacking us
	UFUNCTION(BlueprintCallable, Category = "Planner")
	TArray<AActor*> GetAttackingThreats();

	virtual void SetCommonLocalState(TMap<FString, bool>* LocalState) override;

	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
