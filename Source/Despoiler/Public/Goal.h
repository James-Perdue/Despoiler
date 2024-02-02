// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Goal.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom))
class DESPOILER_API UGoal : public UObject
{
	GENERATED_BODY()

public:

	UGoal();

	TMap<FString, bool> GetDesiredState() 
	{ 
		//TMap<FString, bool> emptyState; 
		return DesiredState;
	}
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GOAP")
	bool isValid(AActor* actor);
	virtual bool isValid_Implementation(AActor* actor) { return true; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GOAP")
	int GetPriority(AActor* actor);
	virtual int GetPriority_Implementation(AActor* actor) { return Priority; }
	
	protected:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
		int Priority;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
		TMap<FString, bool> DesiredState;
};
