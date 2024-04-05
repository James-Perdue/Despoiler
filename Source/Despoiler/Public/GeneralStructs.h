// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <string>
#include <GameplayTagContainer.h>
#include "GeneralEnums.h"
#include "GeneralStructs.generated.h"

//Forward Declarations
class AGoalAICharacter;
class UWeaponComponent;


USTRUCT(BlueprintType)
struct DESPOILER_API FDamageInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = General)
	int Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = General)
	AActor* DamagingActor;

	/*FDamageInfo();

	FDamageInfo(int newDamage, AActor* newDamagingActor)
	{
		Damage = newDamage;
		DamagingActor = newDamagingActor;
	}*/
};

USTRUCT(BlueprintType)
struct DESPOILER_API FFormationInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Formation)
	int FormationWidth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Formation)
	int NumRows;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Formation)
	int NumColumns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Formation)
	AGoalAICharacter* Leader;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Formation)
	EFormation FormationType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Formation)
	float FormationSpacing;
};

USTRUCT(BlueprintType)
struct DESPOILER_API FColumn
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Formation)
	TArray<AGoalAICharacter*> Members;
};

USTRUCT(BlueprintType)
struct DESPOILER_API FCharacterData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
	ECharacterType CharacterType;

	//TODO: add WeaponData for spawning custom weapons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
	TSubclassOf<UWeaponComponent> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
	int Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
	float Armor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
	int Count;
};