// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLAttributeComponent.h"
#include "ARLGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

// Sets default values for this component's properties
UARLAttributeComponent::UARLAttributeComponent()
{
	HealthMax = 100.0f;
	Health = HealthMax;
	Rage = 0;
	RageMax = 100;
}

UARLAttributeComponent* UARLAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UARLAttributeComponent>(FromActor->GetComponentByClass(UARLAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool UARLAttributeComponent::IsActorAlive(AActor* FromActor)
{
	UARLAttributeComponent* AttrbuteComp = GetAttributes(FromActor);
	if (AttrbuteComp)
	{
		return AttrbuteComp->IsAlive();
	}

	return false;

}

bool UARLAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		const float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}
	
	const float OldHealth = Health;
	
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	const float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	// Died
	if (ActualDelta < 0.f && Health == 0.f)
	{
		AARLGameModeBase* GM = GetWorld()->GetAuthGameMode<AARLGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

	return ActualDelta != 0;
}

bool UARLAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UARLAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

float UARLAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

float UARLAttributeComponent::GetCurrentHealth() const
{
	return Health;
}

float UARLAttributeComponent::GetRage() const
{
	return Rage;
}

bool UARLAttributeComponent::ApplyRage(AActor* InstigatorActor, float Delta)
{
	const float OldRage = Rage;

	Rage = FMath::Clamp(Rage + Delta, 0.0f, RageMax);

	const float ActualDelta = Rage - OldRage;
	if (ActualDelta != 0.0f)
	{
		OnRageChanged.Broadcast(InstigatorActor, this, Rage, ActualDelta);
	}

	return ActualDelta != 0;
}

bool UARLAttributeComponent::Kill(AActor* InstigatorACtor)
{
	return ApplyHealthChange(InstigatorACtor, -GetHealthMax());
}

