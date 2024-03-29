// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <GeneralEnums.h>
#include "Components/ActorComponent.h"
#include "CharacterInteractComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DESPOILER_API UCharacterInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterInteractComponent();

	EActionStatus TryCapture();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interact")
	int InteractSpeed = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	int CaptureDelay = 1;

protected:

	FTimerHandle CaptureTimer;

	// Called when the game starts
	virtual void BeginPlay() override;

	void Capture();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
