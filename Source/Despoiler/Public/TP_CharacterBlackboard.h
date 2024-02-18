// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Despoiler/DespoilerGameMode.h>
#include "GeneralEnums.h"
#include "TP_TeamDetails.h"
#include "TP_CharacterBlackboard.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DESPOILER_API UTP_CharacterBlackboard : public UTP_TeamDetails
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTP_CharacterBlackboard();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Planner")
	AActor* CurrentTarget;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Planner")
	float MinDistance;

	void SetCommonLocalState(TMap<FString, bool>* LocalState);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
