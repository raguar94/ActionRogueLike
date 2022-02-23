// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLMagicProjectile.h"
#include "ARLTeleportProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARLTeleportProjectile : public AARLMagicProjectile
{
	GENERATED_BODY()

public:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TimeBeforeExplode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TimeBeforeTeleport;

	FTimerHandle TimerHandle_Explosion;

	FTimerHandle TimerHandle_Teleport;

	void Explode();

	void Teleport();

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
