// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_PlanningAgent.h"

UTP_PlanningAgent::UTP_PlanningAgent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.SetTickFunctionEnable(true);
}



void UTP_PlanningAgent::BeginPlay()
{
	Super::BeginPlay();
	GoalObjects.Empty();
	for (TSubclassOf<UGoal> goal : PossibleGoals)
	{
		GoalObjects.Add(NewObject<UGoal>(this, goal));
	}

	ActionObjects.Empty();
	for (TSubclassOf<UAction> action : PossibleActions)
	{
		ActionObjects.Add(NewObject<UAction>(this, action));
	}
	GetWorld()->GetTimerManager().SetTimer(CheckGoalTimer, this,  &UTP_PlanningAgent::CheckGoal, CheckGoalFrequency, true, -1);

	CurrentPlanIndex = 0;
}

void UTP_PlanningAgent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentGoal == nullptr)
	{
		CheckGoal();
	}
	//If Plan is no actions, try to find a new one. An empty plan can also mean it is complete
	if(CurrentGoal != nullptr && CurrentPlan.PlanActions.Num() == 0)
	{
		FetchPlan();
	}

	if (CurrentGoal != nullptr && CurrentPlan.PlanActions.Num() > 0)
	{
		FollowPlan(DeltaTime);
	}
	else {
		if (isDebug) {
			UE_LOG(LogTemp, Log, TEXT("Planning agent idle, no goal or plan"));
		}
	}
}

void UTP_PlanningAgent::CheckGoal()
{
	UGoal* bestGoal = GetBestGoal();
	if (CurrentGoal == nullptr || bestGoal != CurrentGoal)
	{
		CurrentGoal = bestGoal;
		if (CurrentGoal != nullptr) {
			FetchPlan();
		}
		//Get New Plan
		
	}
}

void UTP_PlanningAgent::FetchPlan()
{
	if (AGoalAICharacter* character = Cast<AGoalAICharacter>(this->GetOwner()) )
	{
		if (isDebug) {
			UE_LOG(LogTemp, Log, TEXT("Updating state and finding plan"));
		}

		character->Blackboard->SetCommonLocalState(&this->LocalState);
		CurrentPlan = Planner::GetPlan(LocalState, CurrentGoal->GetDesiredState(), ActionObjects, this->GetOwner());
		CurrentPlanIndex = 0;
	}
}

UGoal* UTP_PlanningAgent::GetBestGoal()
{
	UGoal* BestGoal = nullptr;
	int highestPriority = -1;

	for (UGoal* goal : GoalObjects) {
		if (goal->isValid(this->GetOwner()))
		{
			int goalPriority = goal->GetPriority(this->GetOwner());
			if (highestPriority < goalPriority)
			{
				highestPriority = goalPriority;
				BestGoal = goal;
			}
		}
	}
	if (BestGoal == nullptr) {
		if (isDebug) {
			UE_LOG(LogTemp, Warning, TEXT("NO GOAL FOUND"));
		}
	}
	else {
		//UE_LOG(LogTemp, Log, TEXT("Goal Found %s"), *BestGoal->GetName());
	}

	return BestGoal;
}



void UTP_PlanningAgent::FollowPlan(float DeltaTime)
{
	if (CurrentPlan.PlanActions.Num() == 0)
	{
		if (isDebug) {
			UE_LOG(LogTemp, Log, TEXT("No plan to follow"));
		}
		return;
	}
	if (CurrentPlanIndex >= CurrentPlan.PlanActions.Num())
	{
		if (isDebug) {
			UE_LOG(LogTemp, Log, TEXT("Plan complete"));
		}
		CurrentPlan.PlanActions.Empty();
		return;
	}

	EActionStatus currentStepStatus = CurrentPlan.PlanActions[CurrentPlanIndex]->PerformAction(this->GetOwner(), DeltaTime);
	if (currentStepStatus == EActionStatus::Completed)
	{
		if (isDebug) {
			UE_LOG(LogTemp, Log, TEXT("Action of step %d in plan is complete. Goal: %s"), CurrentPlanIndex, *CurrentGoal->GetName());
		}
		GOAPUtil::ApplyState(&LocalState, CurrentPlan.PlanActions[CurrentPlanIndex]->GetEffectState());
		CurrentPlanIndex++;

	}
	else if (currentStepStatus == EActionStatus::Failed)
	{
		if (isDebug) {
			UE_LOG(LogTemp, Log, TEXT("Plan failed, aborting plan"));
		}
		CurrentPlan.PlanActions.Empty();
		return;
	}
}

WorldStateStore::WorldStateStore()
{
}

FPlan Planner::GetPlan(TMap<FString, bool> CurrentState, TMap<FString, bool> GoalState, TArray<UAction*> PossibleActions, AActor* actor)
{
	TMap<FString, bool> GoalCopy = GoalState;
	GOAPUtil::RemoveSatisfied(CurrentState, &GoalCopy);
	
	if (GoalCopy.Num() == 0) {
		return FPlan();
	}

	FPlanNode root;
	root.NodeAction = nullptr;
	if (BuildPlans(GoalCopy, PossibleActions, &root, actor))
	{
		FPlan plan = FindBestPlan(&root);
		return plan;
	}

	return FPlan();
}

bool Planner::BuildPlans(TMap<FString, bool> GoalState, TArray<UAction*> PossibleActions, FPlanNode* PlanTree, AActor* actor)
{
	bool nodeWorks = false;
	//TArray<UAction*> Plan;

	TMap<FString, bool> GoalCopy = GoalState;

	if (GoalCopy.Num() == 0) {
		return true;
	}

	for (UAction* Action : PossibleActions)
	{
		if (Action == PlanTree->NodeAction)
		{
			continue;
		}

		bool actionUsable = false;
		if (Action->isValid(actor) != true) continue;
		TMap<FString, bool> GoalCopyInner = GoalCopy;
		if (GOAPUtil::RemoveSatisfied(Action->GetEffectState(), &GoalCopyInner))
		{
			for (TPair<FString, bool> Precondition : Action->GetPreconditions())
			{
				GoalCopyInner.Add(Precondition.Key, Precondition.Value);
			}
			TSharedPtr<FPlanNode> Node = MakeShared<FPlanNode>(Action);
			//UE_LOG(LogTemp, Log, TEXT("DEBUG LOG: GoalCopy has %d entries"), goals);
			if (GoalCopyInner.Num() <= 0 || BuildPlans(GoalCopyInner, PossibleActions, Node.Get(), actor))
			{
				PlanTree->Children.Add(Node);
				nodeWorks = true;
			}
		}
	}
	return nodeWorks;
}

FPlan Planner::FindBestPlan(FPlanNode* PlanTree)
{
	FPlan bestPlan = FPlan();
	bestPlan.Cost = 1000000;
	TArray<FPlan> plans = TransformPlanTreeToArray(PlanTree);
	for(FPlan plan : plans)
	{
		//print plan
		PrintPlan(plan);
		if (bestPlan.Cost > plan.Cost) 
		{
			bestPlan = plan;
		}
	}
	return bestPlan;
}

TArray<FPlan> Planner::TransformPlanTreeToArray(FPlanNode* PlanTree)
{
	TArray<FPlan> Plans = TArray<FPlan>();

	if (PlanTree->Children.Num() == 0)
	{
		TArray<UAction*> actions = TArray<UAction*>();
		actions.Add(PlanTree->NodeAction);
		FPlan plan = FPlan(actions);
		plan.Cost = PlanTree->NodeAction->GetCost();
		Plans.Add(plan);
		return Plans;
	}

	for (TSharedPtr<FPlanNode> child : PlanTree->Children)
	{
		for (FPlan childplan : TransformPlanTreeToArray(child.Get()))
		{
			if (PlanTree->NodeAction != nullptr)
			{
				childplan.PlanActions.Add(PlanTree->NodeAction);
				childplan.Cost += PlanTree->NodeAction->GetCost();
			}
			Plans.Add(childplan);
		}
	}
	return Plans;
}

void Planner::PrintPlan(FPlan plan)
{
	FString planString = "";
	for (UAction* action : plan.PlanActions)
	{
		planString.Append(FString(action->GetName()));
		planString.Append(FString(", "));
	}
	UE_LOG(LogTemp, Log, TEXT("Plan: %s Cost: %d"), *planString, plan.Cost);
}

FPlanNode::FPlanNode()
{
}

FPlanNode::FPlanNode(UAction* action)
{
	NodeAction = action;
}

FPlan::FPlan(TArray<UAction*> Actions)
{
	PlanActions = Actions;
}

FPlan::FPlan()
{
}
