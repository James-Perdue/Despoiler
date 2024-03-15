// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Templates/SharedPointer.h"
#include "GeneralEnums.h"
#include "Action.h"
#include "Goal.h"
#include "Containers/Map.h"
#include <BlackboardBaseComponent.h>
#include "TP_PlanningAgent.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct DESPOILER_API FPlanNode
{
	GENERATED_BODY()

public:
	FPlanNode();
	FPlanNode(UAction* action);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Planner")
	UAction* NodeAction;

	TArray<TSharedPtr<FPlanNode>> Children;
};

USTRUCT(BlueprintType)
struct DESPOILER_API FPlan
{
	GENERATED_BODY()
public:
	FPlan(TArray<UAction*> Actions);
	FPlan();

	TArray<UAction*> PlanActions;

	int Cost;
};

class DESPOILER_API Planner
{
public:
	//Need method to evaluate states given a goal and current. It needs to be cheap
	static FPlan GetPlan( TMap<FString, bool> CurrentState,  TMap<FString, bool> GoalState,  TArray<UAction*> PossibleActions, AActor* actor);

protected:
	static bool BuildPlans(TMap<FString, bool> GoalState, TArray<UAction*> PossibleActions, FPlanNode* PlanTree, AActor* actor);

	static FPlan FindBestPlan(FPlanNode* PlanTree);

	static TArray<FPlan> TransformPlanTreeToArray(FPlanNode* PlanTree);

	static void PrintPlan(FPlan plan);
};


class DESPOILER_API GOAPUtil 
{
public:
	//Make sure to copy goal state before passing in
	static bool StateSatisfiesAll(TMap<FString, bool> CurrentState, TMap<FString, bool> GoalState)
	{
		for(TPair<FString, bool> pair : GoalState)
		{
			if (!(CurrentState.Contains(pair.Key) && CurrentState[pair.Key] == pair.Value))
			{
				return false;
			}
		}

		return true;
	}

	static bool RemoveSatisfied(TMap<FString, bool> CurrentState, TMap<FString, bool>* GoalState)
	{
		bool satisfies = false;
		TMap<FString, bool> GoalCopy = *GoalState;
		for (TPair<FString, bool> pair : GoalCopy)
		{
			//UE_LOG(LogTemp, Log, TEXT("Checking for GoalState: %s, value %d"), *pair.Key, pair.Value);
			//UE_LOG(LogTemp, Log, TEXT("CurrentState contains key: %d"), CurrentState.Contains(pair.Key));
			if (CurrentState.Contains(pair.Key))
			{
				//UE_LOG(LogTemp, Log, TEXT("CurrentState key has value: %d"), CurrentState[pair.Key]);

				if (CurrentState[pair.Key] == pair.Value)
				{
					satisfies = true;
					GoalState->Remove(pair.Key);
				}
			}
			
		}

		return satisfies;
	}

	static void ApplyState(TMap<FString, bool>* CurrentState, TMap<FString, bool> GoalState)
	{
		for (TPair<FString, bool> pair : GoalState)
		{
			CurrentState->Emplace(pair.Key, pair.Value);
		}
	}
};

class DESPOILER_API WorldStateStore
{
public:
	WorldStateStore();
	AActor* TargetActor;
	bool InCombat;
	bool AtTarget;
	bool Attacking;
	bool Idle;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DESPOILER_API UTP_PlanningAgent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Planner")
	TArray<TSubclassOf<UGoal>> PossibleGoals;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Planner")
	TArray<TSubclassOf<UAction>> PossibleActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Planner")
	float CheckGoalFrequency = .25f;

	UFUNCTION(BlueprintCallable, Category = "Planner")
	void SetRefreshRate(float rate);

	UFUNCTION(BlueprintCallable, Category = "Planner")
	void ResetRefreshRate();

	UFUNCTION(BlueprintCallable, Category = "Planner")
	void StopPlanning();

	UFUNCTION(BlueprintCallable, Category = "Planner")
	void StartPlanning();


	UTP_PlanningAgent();
	UGoal* GetCurrentGoal() { return CurrentGoal; }
	UGoal* GetBestGoal();

protected:
	UPROPERTY(VisibleAnywhere)
	UGoal* CurrentGoal;
	UPROPERTY(VisibleAnywhere)
	FPlan CurrentPlan;

	Planner Planner;
	UPROPERTY(VisibleAnywhere)
	TMap<FString, bool> LocalState;

	FTimerHandle CheckGoalTimer;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void CheckGoal();
	void FetchPlan();

	void FollowPlan(float DeltaTime);
	//bool IsStateSatisfied(TMap<FString, bool> CurrentState, TMap<FString, bool> DesiredState);

private:
	UPROPERTY()
	TArray<UGoal*> GoalObjects;
	UPROPERTY()
	TArray<UAction*> ActionObjects;
	int CurrentPlanIndex = 0;

	UPROPERTY(EditAnywhere)
	bool isDebug = false;
};
