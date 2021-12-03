// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLTargetDummy.h"
#include "Components/StaticMeshComponent.h"
#include "ARLAttributeComponent.h"

// Sets default values
AARLTargetDummy::AARLTargetDummy()
{
 	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;
	
	AttributeComponent = CreateDefaultSubobject<UARLAttributeComponent>("AttributeComponent");
	// Trigger when Health is changed (Damage/healing)
	AttributeComponent->OnHealthChanged.AddDynamic(this, &AARLTargetDummy::OnHealthChanged);
}

void AARLTargetDummy::OnHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}

