// Fill out your copyright notice in the Description page of Project Settings.


#include "DespoilerGameInstance.h"
#include "WeaponComponent.h"


UDespoilerGameInstance::UDespoilerGameInstance()
{

}

FFormationInfo UDespoilerGameInstance::GetDefenderFormation()
{
	FFormationInfo formationInfo = FFormationInfo();
	formationInfo.FormationWidth = 10;
	formationInfo.FormationSpacing = 150;
	switch (SaveData.Level)
	{
	case 0:

	default:
		break;
	}
	return formationInfo;
}

TArray<FCharacterData> UDespoilerGameInstance::GetDefenderUnitData()
{

	FCharacterData data = FCharacterData();
	switch (SaveData.Level)
	{
	case 0:
		data.Count = 10;
		break;
	case 1:
		data.Count = 20;
		break;
	case 2:
		data.Count = 40;
	default:
		break;
	}
	data.WeaponClass = Sword;
	data.Armor = 1;
	data.Health = 2;
	TArray<FCharacterData> list = TArray<FCharacterData>();
	list.Add(data);
	return list;
}

void UDespoilerGameInstance::Init()
{
	isSaved = true;
	PlayerFormationData = FFormationInfo();
	PlayerUnitData = TArray<FCharacterData>();

	PlayerFormationData.FormationSpacing = 125;
	PlayerFormationData.FormationWidth = 5;

	SaveData = FSaveData();
	SaveData.Gold = 3;
	SaveData.MaxUnits = 10;
	SaveData.ArmorRank = 1;
	SaveData.WeaponRank = 1;
}
