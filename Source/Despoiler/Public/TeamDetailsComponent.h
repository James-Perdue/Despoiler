// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <GeneralEnums.h>
#include <Despoiler/DespoilerGameMode.h>
#include <Team.h>
#include "TeamDetailsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DESPOILER_API UTeamDetailsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTeamDetailsComponent();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Planner")
	ETeam TeamAssignment;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Planner")
	ATeam* MyTeam;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Planner")
	ATeam* OpposingTeam;

protected:
	//FTimerHandle AssignTeamTimer;

	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void AssignTeam();


public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
