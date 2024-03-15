// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <string>
#include <GameplayTagContainer.h>
#include "GeneralEnums.h"
#include "GeneralUtil.generated.h"

// Forward Declarations:
class ASquad;

UCLASS()
class UGeneralUtil : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Util")
	static AActor* GetClosestActor(FVector location, TArray<AActor*> actors);

	UFUNCTION(BlueprintCallable, Category = "Util")
	static FVector GetClosestPoint(FVector location, TArray<FVector> points);

	UFUNCTION(BlueprintCallable, Category = "Util")
	static bool IsSquadStateEqual(ASquad* squad, ESquadState goalState);
};