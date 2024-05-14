// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaGameMode.h"
#include <Kismet/GameplayStatics.h>
#include <DespoilerGameInstance.h>

AMetaGameMode::AMetaGameMode()
{

}

void AMetaGameMode::InitAttackerData(int width, int spacing, int health, int count, TSubclassOf<UWeaponComponent> weapon)
{
	PlayerFormationData.FormationSpacing = spacing;
	PlayerFormationData.FormationWidth = width;
	FCharacterData CharacterData = FCharacterData();
	CharacterData.Health = health;
	CharacterData.Count = count;
	CharacterData.WeaponClass = weapon;
	PlayerUnitData.Add(CharacterData);

	UDespoilerGameInstance* instance = Cast<UDespoilerGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	instance->SetPlayerFormation(PlayerFormationData);
	instance->SetPlayerUnitData(PlayerUnitData);
	instance->isSaved = true;
	FSaveData saveData = instance->GetSaveData();
	saveData.MaxUnits = count;
	saveData.ArmorRank = health;
	saveData.WeaponRank = 1;
	instance->SetSaveData(saveData);
}

void AMetaGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (UDespoilerGameInstance* instance = Cast<UDespoilerGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		if (!instance->isSaved) {
			FSaveData data = FSaveData();
			data.Gold = 3;
			data.ArmorRank = 1;
			data.WeaponRank = 1;
			data.MaxUnits = 10;
			instance->SetSaveData(data);
		}
		
	}
	

	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
	}
}
