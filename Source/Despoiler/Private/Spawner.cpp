// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InitialWaveDelay = 5;
	WaveDelay = 5;
	SpawnRadius = 5;
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(WaveTimer, this, &ASpawner::SpawnWave, WaveDelay, true, InitialWaveDelay);
}

void ASpawner::SpawnWave()
{
	for (const auto& SpawnPair : SpawnMap) {
		TSubclassOf<ACharacter> SpawnEnemy = SpawnPair.Key;
		int SpawnAmount = SpawnPair.Value;
		for (int i = 0; i < SpawnAmount; i++) {
			FVector2D RandomPoint = FMath::RandPointInCircle(SpawnRadius);
			FVector SpawnPoint = FVector(this->GetActorLocation().X + RandomPoint.X, this->GetActorLocation().Y + RandomPoint.Y, this->GetActorLocation().Z);
			UWorld* World = this->GetWorld();
			FActorSpawnParameters spawnParameters;

			spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			ACharacter* Enemy = World->SpawnActor<ACharacter>(SpawnEnemy, SpawnPoint, FRotator::ZeroRotator);
			if (Enemy != nullptr) {
				Enemy->SpawnDefaultController();
			}
		}
	}
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

