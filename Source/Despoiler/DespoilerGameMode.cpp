// Copyright Epic Games, Inc. All Rights Reserved.

#include "DespoilerGameMode.h"
#include "DespoilerPlayerController.h"
#include "DespoilerCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include <Logging/StructuredLog.h>

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

void ADespoilerGameMode::EndGame(ETeam losingTeam)
{

	if (losingTeam == ETeam::Attacker)
	{
		GameEndDelegate.Broadcast(ETeam::Defender);
	}
		//UE_LOGFMT(LogTemp, Log, "Team Wins: Defender");
		

	if (losingTeam == ETeam::Defender)
	{
		//GameEndDelegate.Broadcast(ETeam::Attacker);
	}
		//UE_LOGFMT(LogTemp, Log, "Team Wins: Attacker");

}

void ADespoilerGameMode::UpdateCaptureProgress(int amountCaptured)
{
	CaptureProgress += amountCaptured;
	CaptureProgressUpdateDelegate.Broadcast(CaptureProgress);
	if (CaptureProgress >= 100)
	{
		GameEndDelegate.Broadcast(ETeam::Attacker);
	}
}

void ADespoilerGameMode::BeginPlay()
{
	FActorSpawnParameters SpawnParams;
	ATeam* AttackingTeam = GetWorld()->SpawnActor<ATeam>(ATeam::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	AttackingTeam->Team = ETeam::Attacker;
	Teams.Add(ETeam::Attacker, AttackingTeam);
	AttackingTeam->TeamDefeatedDelegate.AddDynamic(this, &ADespoilerGameMode::EndGame);

	ATeam* DefendingTeam = GetWorld()->SpawnActor<ATeam>(ATeam::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	DefendingTeam->Team = ETeam::Defender;
	Teams.Add(ETeam::Defender, DefendingTeam);
	DefendingTeam->TeamDefeatedDelegate.AddDynamic(this, &ADespoilerGameMode::EndGame);

	GameInitDelegate.Broadcast();
	TeamUpdateDelegate.Broadcast();
}


