// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLPowerUp_HealthPotion.h"
#include "ARLAttributeComponent.h"
#include "ARLPlayerState.h"

AARLPowerUp_HealthPotion::AARLPowerUp_HealthPotion()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	// Disable collision, instead we use sphereCopm to handle interaction queries
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetupAttachment(RootComponent);

	CreditCost = 50;

}

void AARLPowerUp_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}
	
	UARLAttributeComponent* AttributeComp = Cast<UARLAttributeComponent>(InstigatorPawn->GetComponentByClass(UARLAttributeComponent::StaticClass()));
	// check if not already at max health
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		if (AARLPlayerState* PS = InstigatorPawn->GetPlayerState<AARLPlayerState>())
		{
			// Only activate if healed successfully
			if (PS->RemoveCredits(CreditCost) && AttributeComp->ApplyHealthChange(this, AttributeComp->GetHealthMax()))
			{
				HideAndCooldownPowerUp();
			}
		}
	}
}
