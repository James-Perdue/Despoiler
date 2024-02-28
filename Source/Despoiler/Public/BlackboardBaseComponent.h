// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TeamDetailsComponent.h"
#include "BlackboardBaseComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DESPOILER_API UBlackboardBaseComponent : public UTeamDetailsComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBlackboardBaseComponent();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Planner")
	AActor* CurrentTarget;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetCommonLocalState(TMap<FString, bool>* LocalState);
};
