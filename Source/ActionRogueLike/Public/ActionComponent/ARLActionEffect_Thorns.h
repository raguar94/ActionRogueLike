// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionComponent/ARLActionEffect.h"
#include "ARLActionEffect_Thorns.generated.h"

class UARLAttributeComponent;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARLActionEffect_Thorns : public UARLActionEffect
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Thorns")
	float ReflectFraction;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwningComp, float NewHealth, float Delta);

public:

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;

	UARLActionEffect_Thorns();
	
};
