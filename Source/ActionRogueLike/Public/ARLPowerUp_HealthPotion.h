// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLPowerUpActor.h"
#include "ARLPowerUp_HealthPotion.generated.h"

class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARLPowerUp_HealthPotion : public AARLPowerUpActor
{
	GENERATED_BODY()

protected:

	// Static mesh component
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly)
	int32 CreditCost;

public:

	AARLPowerUp_HealthPotion();

	void Interact_Implementation(APawn* InstigatorPawn) override;
	
};
