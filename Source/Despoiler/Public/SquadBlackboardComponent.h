// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <BlackboardBaseComponent.h>
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

	//AActor* Leader

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Planner")
	TArray<AGoalAICharacter*> Members;

	virtual void SetCommonLocalState(TMap<FString, bool>* LocalState) override; 

	UFUNCTION(BlueprintCallable, Category = Planner)
	void RemoveMember(AGoalAICharacter* member);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void AssignTeam() override;


public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
