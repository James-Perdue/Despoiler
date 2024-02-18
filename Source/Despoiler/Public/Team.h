// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <GeneralEnums.h>
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

	UFUNCTION( BlueprintCallable, Category = "GOAP")
	AActor* GetClosestMember(FVector location);

	UFUNCTION(BlueprintCallable, Category = "GOAP")
	void AddMember(AActor* member);

	UFUNCTION(BlueprintCallable, Category = "GOAP")
	void RemoveMember(AActor* member);

	UFUNCTION(BlueprintCallable, Category = "GOAP")
	void SetObjective(AActor* actor);

	ETeam Team;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Planner")
	TArray<AActor*> Members;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Planner")
	AActor* CurrentObjective;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
