// Fill out your copyright notice in the Description page of Project Settings.


#include "PositionMarker.h"
#include <Despoiler/DespoilerGameMode.h>

// Sets default values
APositionMarker::APositionMarker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	TeamAssignment = ETeam::Attacker;
}

bool APositionMarker::isOccupied()
{
	return false;
}

// Called when the game starts or when spawned
void APositionMarker::BeginPlay()
{
	Super::BeginPlay();
	
	ADespoilerGameMode* mode = Cast<ADespoilerGameMode>(this->GetWorld()->GetAuthGameMode());
	if (mode != nullptr)
	{
		mode->GameInitDelegate.AddDynamic(this, &APositionMarker::Register);
	}
}

void APositionMarker::Register()
{
	ADespoilerGameMode* mode = Cast<ADespoilerGameMode>(this->GetWorld()->GetAuthGameMode());
	if (mode != nullptr)
	{
		mode->Teams[TeamAssignment]->AddPosition(this);
	}
}

// Called every frame
void APositionMarker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

