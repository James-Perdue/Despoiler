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

	UDespoilerGameInstance();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFormationInfo PlayerFormationData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCharacterData> PlayerUnitData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSaveData SaveData;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isSaved = false;

	UFUNCTION(BlueprintCallable)
	void SetPlayerFormation(FFormationInfo formationData) { PlayerFormationData = formationData; }

	UFUNCTION(BlueprintCallable)
	void SetPlayerUnitData(TArray<FCharacterData> unitData) { PlayerUnitData = unitData; }

	UFUNCTION(BlueprintCallable)
	void SetSaveData(FSaveData data) { SaveData = data; }

	UFUNCTION(BlueprintCallable)
	void IncreaseLevel() { 
		SaveData.Level += 1;
		if (SaveData.Level >= 2)
		{
			SaveData.Level = 0;
			SaveData.Stage++;
		}
	}

	UFUNCTION(BlueprintCallable)
	FSaveData GetSaveData() { return SaveData; }

	UFUNCTION(BlueprintCallable)
	FFormationInfo GetPlayerFormation() { return PlayerFormationData; }

	UFUNCTION(BlueprintCallable)
	TArray<FCharacterData> GetPlayerUnitData() { return PlayerUnitData; }

	UFUNCTION(BlueprintCallable)
	FFormationInfo GetDefenderFormation();

	UFUNCTION(BlueprintCallable)
	TArray<FCharacterData> GetDefenderUnitData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UWeaponComponent> Sword;

	virtual void Init() override;
};
