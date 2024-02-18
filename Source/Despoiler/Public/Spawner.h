// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Spawner.generated.h"

UCLASS()
class DESPOILER_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaveDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitialWaveDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TSubclassOf<class ACharacter>, int> SpawnMap;

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle WaveTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnWave();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
