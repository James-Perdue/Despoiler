// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <Team.h>
#include "GameFramework/GameModeBase.h"
#include "DespoilerGameMode.generated.h"

UCLASS(minimalapi)
class ADespoilerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADespoilerGameMode();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Planner")
	TMap<ETeam, ATeam*> Teams;

	virtual void BeginPlay() override;
};



