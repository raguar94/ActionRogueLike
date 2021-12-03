// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLAttributeComponent.h"

// Sets default values for this component's properties
UARLAttributeComponent::UARLAttributeComponent()
{
	HealthMax = 100.0f;
	Health = HealthMax;
}

bool UARLAttributeComponent::ApplyHealthChange(float Delta)
{
	float OldHealth = Health;
	
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(nullptr, this, Health, ActualDelta);

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

