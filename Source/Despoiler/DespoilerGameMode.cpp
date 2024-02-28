// Copyright Epic Games, Inc. All Rights Reserved.

#include "DespoilerGameMode.h"
#include "DespoilerPlayerController.h"
#include "DespoilerCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADespoilerGameMode::ADespoilerGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ADespoilerPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Custom/Blueprints/Controller/Player"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Custom/Blueprints/Controller/PlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}

void ADespoilerGameMode::BeginPlay()
{
	FActorSpawnParameters SpawnParams;
	ATeam* AttackingTeam = GetWorld()->SpawnActor<ATeam>(ATeam::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	AttackingTeam->Team = ETeam::Attacker;
	Teams.Add(ETeam::Attacker, AttackingTeam);

	ATeam* DefendingTeam = GetWorld()->SpawnActor<ATeam>(ATeam::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	DefendingTeam->Team = ETeam::Defender;
	Teams.Add(ETeam::Defender, DefendingTeam);

}
