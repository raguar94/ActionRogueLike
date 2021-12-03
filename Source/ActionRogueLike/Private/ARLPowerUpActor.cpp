// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLPowerUpActor.h"
#include "Components/SphereComponent.h"

// Sets default values
AARLPowerUpActor::AARLPowerUpActor()
{
 	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("PowerUp");
	RootComponent = SphereComp;

	RespawnTime = 10.0f;
}

void AARLPowerUpActor::Interact_Implementation(APawn* InstigatorPawn)
{
	// Logic in derived classes...
}

void AARLPowerUpActor::ShowPowerUp()
{
	SetPowerUpState(true);
}

void AARLPowerUpActor::HideAndCooldownPowerUp()
{
	SetPowerUpState(false);

	GetWorldTimerManager().SetTimer(TimerHandle_RaspwnTimer, this, &AARLPowerUpActor::ShowPowerUp, RespawnTime);
}

void AARLPowerUpActor::SetPowerUpState(bool bNewIsActive)
{
	SetActorEnableCollision(bNewIsActive);
	
	// Set visibility on root and all children
	RootComponent->SetVisibility(bNewIsActive, true);
}

