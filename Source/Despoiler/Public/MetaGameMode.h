// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GeneralEnums.h"
#include "GeneralStructs.h"
#include "MetaGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLoadStageDelegate);

/**
 * 
 */
UCLASS()
class DESPOILER_API AMetaGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AMetaGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFormationInfo PlayerFormationData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCharacterData> PlayerUnitData;

	UPROPERTY(BlueprintAssignable)
	FLoadStageDelegate LoadStageDelegate;

	UFUNCTION(BlueprintCallable)
	void InitAttackerData(int width, int spacing, int health, int count, TSubclassOf<UWeaponComponent> weapon);

protected:

	virtual void BeginPlay() override;

};
