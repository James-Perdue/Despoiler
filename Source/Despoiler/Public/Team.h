// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <GeneralEnums.h>
#include <GeneralUtil.h>
#include <PositionMarker.h>
#include "Team.generated.h"

UCLASS(ClassGroup = (Custom))
class DESPOILER_API ATeam : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeam();

	UFUNCTION(BlueprintCallable, Category = "GOAP")
	AActor* GetObjective() { return CurrentObjective; }

	UFUNCTION(BlueprintCallable, Category = "GOAP")
	TArray<AActor*> GetMembers() { return Members; }

	UFUNCTION(BlueprintCallable, Category = "GOAP")
	TArray<ASquad*> GetSquads() { return Squads; }

	UFUNCTION( BlueprintCallable, Category = "GOAP")
	AActor* GetClosestMember(FVector location);

	UFUNCTION(BlueprintCallable, Category = "GOAP")
	void AddMember(AActor* member);

	UFUNCTION(BlueprintCallable, Category = "GOAP")
	void RemoveMember(AActor* member);

	UFUNCTION(BlueprintCallable, Category = "GOAP")
	void AddSquad(ASquad* squad);

	UFUNCTION(BlueprintCallable, Category = "GOAP")
	void RemoveSquad(ASquad* squad);

	UFUNCTION(BlueprintCallable, Category = "GOAP")
	void SetObjective(AActor* actor);

	UFUNCTION(BlueprintCallable, Category = "GOAP")
	TArray<APositionMarker*> GetPositions() { return Positions; }

	UFUNCTION(BlueprintCallable, Category = "GOAP")
	void AddPosition(APositionMarker* member);

	UFUNCTION(BlueprintCallable, Category = "GOAP")
	void RemovePosition(APositionMarker* member);

	ETeam Team;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Planner")
	TArray<AActor*> Members;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Planner")
	TArray<ASquad*> Squads;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Planner")
	TArray<APositionMarker*> Positions;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Planner")
	AActor* CurrentObjective;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
