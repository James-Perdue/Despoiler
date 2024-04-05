// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include <GeneralStructs.h>
#include "DespoilerGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DESPOILER_API UDespoilerGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFormationInfo PlayerFormationData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCharacterData> PlayerUnitData;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isSaved = false;

	UFUNCTION(BlueprintCallable)
	void SetPlayerFormation(FFormationInfo formationData) { PlayerFormationData = formationData; }

	UFUNCTION(BlueprintCallable)
	void SetPlayerUnitData(TArray<FCharacterData> unitData) { PlayerUnitData = unitData; }

	UFUNCTION(BlueprintCallable)
	FFormationInfo GetPlayerFormation() { return PlayerFormationData; }

	UFUNCTION(BlueprintCallable)
	TArray<FCharacterData> GetPlayerUnitData() { return PlayerUnitData; }

};
