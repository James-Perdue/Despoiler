// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "GeneralEnums.h"
#include "WeaponComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), Blueprintable)
class DESPOILER_API UWeaponComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UWeaponComponent();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	int Damage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	int Range;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	float Warmup;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	float Cooldown;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	EWeaponType WeaponType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	USoundBase* HitSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	USoundBase* BlockSound;


protected:
	/*UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	class UStaticMeshComponent* StaticMeshComponent;*/

	virtual void BeginPlay() override;

};
