// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <GeneralEnums.h>
#include "Action.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom))
class DESPOILER_API UAction : public UObject
{
	GENERATED_BODY()

public:
	UAction();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GOAP")
	bool isValid(AActor* actor);
	virtual bool isValid_Implementation(AActor* actor) { return true; }

	float GetCost() { return Cost; }
	void SetCost(float newCost) { Cost = newCost; }

	//EActionStatus GetStatus() { return Status; }
	TMap<FString, bool> GetEffectState() { return EffectState; }
	TMap<FString, bool> GetPreconditions() { return Preconditions; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GOAP")
	EActionStatus PerformAction(AActor* actor, float delta);
	virtual EActionStatus PerformAction_Implementation(AActor* actor, float delta) { return EActionStatus::Completed; }
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
	float Cost = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
	TMap<FString, bool> EffectState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
	TMap<FString, bool> Preconditions;
};
