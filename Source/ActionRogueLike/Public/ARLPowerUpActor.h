// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARLGameplayInterface.h"
#include "ARLPowerUpActor.generated.h"

class USphereComponent;

UCLASS()
class ACTIONROGUELIKE_API AARLPowerUpActor : public AActor, public IARLGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AARLPowerUpActor();

	void Interact_Implementation(APawn* InstigatorPawn) override;

protected:

	UPROPERTY(EditAnywhere, Category = "PowerUp")
	float RespawnTime;

	FTimerHandle TimerHandle_RaspwnTimer;

	UFUNCTION()
	void ShowPowerUp();

	void HideAndCooldownPowerUp();

	void SetPowerUpState(bool bNewIsActive);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

};
