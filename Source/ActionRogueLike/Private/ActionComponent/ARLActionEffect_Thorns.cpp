// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionComponent/ARLActionEffect_Thorns.h"
#include "ARLAttributeComponent.h"
#include "ARLGameplayFunctionLibrary.h"
#include "ActionComponent/ARLActionComponent.h"

UARLActionEffect_Thorns::UARLActionEffect_Thorns()
{
	ReflectFraction = 0.2f;

	Duration = 0.0f;
	Period = 0.0f;
}

void UARLActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	// Start listening
	UARLAttributeComponent* Attributes = UARLAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.AddDynamic(this, &UARLActionEffect_Thorns::OnHealthChanged);
	}
}

void UARLActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	// Stop listening
	UARLAttributeComponent* Attributes = UARLAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.RemoveDynamic(this, &UARLActionEffect_Thorns::OnHealthChanged);
	}
}

void UARLActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();

	// Damage Only
	if (Delta < 0.0f && OwningActor != InstigatorActor)
	{
		// Round to nearest to avoid 'ugly' damage numbers and tiny reflections
		int32 ReflectedAmount = FMath::RoundToInt(Delta * ReflectFraction);
		if (ReflectedAmount == 0)
		{
			return;
		}

		// Flip to positive, so we don't end up healing ourselves when passed into damage
		ReflectedAmount = FMath::Abs(ReflectedAmount);

		// Return damage sender...
		UARLGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectedAmount);
	}
}
