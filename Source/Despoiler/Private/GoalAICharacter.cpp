// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalAICharacter.h"

// Sets default values
AGoalAICharacter::AGoalAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGoalAICharacter::BeginPlay()
{
	Super::BeginPlay();
	Blackboard = this->FindComponentByClass<UTP_CharacterBlackboard>();
}

// Called every frame
void AGoalAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGoalAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

