// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <string>
#include <GameplayTagContainer.h>
#include "GeneralEnums.h"
#include "GeneralStructs.generated.h"


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