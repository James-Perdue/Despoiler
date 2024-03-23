// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeneralEnums.h"
#include "PositionMarker.generated.h"

UCLASS()
class DESPOILER_API APositionMarker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APositionMarker();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ETeam TeamAssignment;

	bool isOccupied();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void Register();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
