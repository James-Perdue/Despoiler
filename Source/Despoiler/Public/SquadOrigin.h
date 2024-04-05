// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Squad.h"
#include "GeneralEnums.h"
#include "SquadOrigin.generated.h"

UCLASS()
class DESPOILER_API ASquadOrigin : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASquadOrigin();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Squad)
	TSubclassOf<class ASquad> SquadBlueprint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
