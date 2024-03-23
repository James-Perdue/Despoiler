// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <BlackboardBaseComponent.h>
#include "Logging/StructuredLog.h"
#include "SquadBlackboardComponent.generated.h"
// Forward Declarations:
class AGoalAICharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DESPOILER_API USquadBlackboardComponent : public UBlackboardBaseComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USquadBlackboardComponent();

	//Formation AFormationClass??

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Planner")
	ESquadState SquadState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Planner")
	TArray<AGoalAICharacter*> Members;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Formation")
	EFormation Formation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Formation")
	float FormationSpacing;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Planner")
	float Cohesion = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Formation")
	int FormationWidth;

	UPROPERTY(VisibleAnywhere, Category = "Formation")
	TMap<AGoalAICharacter*, FVector> FormationMap;

	UPROPERTY(VisibleAnywhere, Category = "Formation")
	AGoalAICharacter* Leader;

	FTimerHandle UpdateFormationTimer;

	virtual void SetCommonLocalState(TMap<FString, bool>* LocalState) override; 

	UFUNCTION(BlueprintCallable, Category = "Planner")
	void RemoveMember(AGoalAICharacter* member);

	UFUNCTION(BlueprintCallable, Category = "Planner")
	void SetFormation();

	UFUNCTION(BlueprintCallable, Category = "Planner")
	FVector FetchFormationLocation(AGoalAICharacter* member);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void AssignTeam() override;


public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
