// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLPowerUpActor.h"
#include "ARLPowerUp_Credits.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARLPowerUp_Credits : public AARLPowerUpActor
{
	GENERATED_BODY()

public:


	AARLPowerUp_Credits();
	
protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "Credits")
	int32 CreditsAmount;

public:

	void Interact_Implementation(APawn* InstigatorPawn) override;

};
