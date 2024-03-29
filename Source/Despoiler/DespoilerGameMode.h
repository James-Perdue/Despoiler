// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <Team.h>
#include "GameFramework/GameModeBase.h"
#include "DespoilerGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeathDelegate, AGoalAICharacter*, Actor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCaptureProgressUpdateDelegate, int, Progress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameEndDelegate, ETeam, WinningTeam);


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTeamUpdateDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameInitDelegate);






UCLASS(minimalapi)
class ADespoilerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADespoilerGameMode();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Planner")
	TMap<ETeam, ATeam*> Teams;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Stats)
	int CaptureProgress;

	UPROPERTY(BlueprintAssignable)
	FDeathDelegate DeathDelegate;

	UPROPERTY(BlueprintAssignable)
	FTeamUpdateDelegate TeamUpdateDelegate;

	UPROPERTY(BlueprintAssignable)
	FGameInitDelegate GameInitDelegate;

	UPROPERTY(BlueprintAssignable)
	FGameEndDelegate GameEndDelegate;

	UPROPERTY(BlueprintAssignable)
	FCaptureProgressUpdateDelegate CaptureProgressUpdateDelegate;

	UFUNCTION()
	void EndGame(ETeam losingTeam);

	UFUNCTION(BlueprintCallable)
	void UpdateCaptureProgress(int amountCaptured);

	virtual void BeginPlay() override;
};



