// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadOrigin.h"
#include <DespoilerGameInstance.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
ASquadOrigin::ASquadOrigin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASquadOrigin::BeginPlay()
{
	Super::BeginPlay();
	
	UDespoilerGameInstance* instance = Cast<UDespoilerGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!instance) {
		return;
	}
	ASquad* spawnedSquad = nullptr;

	FVector SpawnPoint = this->GetActorLocation();
	FActorSpawnParameters spawnParameters;

	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	const FTransform SpawnTransform = FTransform(GetActorRotation(), SpawnPoint);
	FFormationInfo formation;
	TArray<FCharacterData> characterData;

	if (TeamAssignment == ETeam::Attacker) {
		formation = instance->GetPlayerFormation();
		characterData = instance->GetPlayerUnitData();
	} 
	if (TeamAssignment == ETeam::Defender) {
		formation = instance->GetDefenderFormation();
		characterData = instance->GetDefenderUnitData();
	}


	spawnedSquad = GetWorld()->SpawnActorDeferred<ASquad>(SquadBlueprint, SpawnTransform);
	if (instance->isSaved || formation.FormationWidth > 0 && characterData.Num() > 0)
	{
		spawnedSquad->SquadBlackboard->FormationInfo = formation;
		spawnedSquad->SquadInitCharacterData = characterData;
		spawnedSquad->SquadBlackboard->TeamAssignment = TeamAssignment;
	}
	spawnedSquad->FinishSpawning(SpawnTransform);
	//spawnedSquad->SetActorLocation(this->GetActorLocation());
	
}

// Called every frame
void ASquadOrigin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

