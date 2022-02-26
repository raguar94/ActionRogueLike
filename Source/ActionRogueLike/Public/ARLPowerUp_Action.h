// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLPowerUpActor.h"
#include "ARLPowerUp_Action.generated.h"

class UARLAction;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARLPowerUp_Action : public AARLPowerUpActor
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, Category = "Powerup")
	TSubclassOf<UARLAction> ActionToGrant;

public:

	void Interact_Implementation(APawn* InstigatorPawn) override;
	
};
