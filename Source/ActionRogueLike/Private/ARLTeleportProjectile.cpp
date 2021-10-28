// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLTeleportProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

void AARLTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetSphereComponent()->IgnoreActorWhenMoving(GetInstigator(), true);

	GetMovementComponent()->ProjectileGravityScale = 0.0f;

	GetSphereComponent()->OnComponentHit.AddDynamic(this, &AARLTeleportProjectile::OnComponentHit);

	GetWorldTimerManager().SetTimer(TimerHandle_Explosion, this, &AARLTeleportProjectile::Explode, TimeBeforeExplode);
}

void AARLTeleportProjectile::Explode()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionParticle, GetActorLocation(), GetActorRotation(), true);

	GetMovementComponent()->StopMovementImmediately();

	GetWorldTimerManager().SetTimer(TimerHandle_Teleport, this, &AARLTeleportProjectile::Teleport, TimeBeforeTeleport);
}

void AARLTeleportProjectile::Teleport()
{
	GetInstigator()->TeleportTo(GetActorLocation(), GetInstigator()->GetActorRotation());

	Destroy();
}

void AARLTeleportProjectile::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}
