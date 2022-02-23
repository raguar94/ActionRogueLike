// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLPowerUp_Credits.h"
#include "ARLPlayerState.h"

AARLPowerUp_Credits::AARLPowerUp_Credits()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	// Disable collision, instead we use SphereComp to handle interaction queries
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	CreditsAmount = 80;
}

void AARLPowerUp_Credits::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	if (AARLPlayerState* PS = InstigatorPawn->GetPlayerState<AARLPlayerState>())
	{
		PS->AddCredits(CreditsAmount);
		HideAndCooldownPowerUp();
	}
}
