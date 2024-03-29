// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterInteractComponent.h"
#include <Despoiler/DespoilerGameMode.h>

// Sets default values for this component's properties
UCharacterInteractComponent::UCharacterInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

EActionStatus UCharacterInteractComponent::TryCapture()
{
	UWorld* world = this->GetOwner()->GetWorld();
	if (!world->GetTimerManager().IsTimerActive(CaptureTimer))
	{
		Capture();
		world->GetTimerManager().SetTimer(CaptureTimer, CaptureDelay, false);
		return EActionStatus::Completed;
	}
	return EActionStatus::Running;
}


// Called when the game starts
void UCharacterInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UCharacterInteractComponent::Capture()
{
	ADespoilerGameMode* mode = Cast<ADespoilerGameMode>(this->GetOwner()->GetWorld()->GetAuthGameMode());

	if (mode != nullptr)
	{
		mode->UpdateCaptureProgress(InteractSpeed);
	}
}


// Called every frame
void UCharacterInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

