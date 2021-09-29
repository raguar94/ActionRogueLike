// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AARLExplosiveBarrel::AARLExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	MeshComponent->SetCollisionProfileName("PhysicsActor");
	MeshComponent->SetSimulatePhysics(true);
	RootComponent = MeshComponent;

	RadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("Radial Force"));
	RadialForce->SetupAttachment(MeshComponent);
	RadialForce->ImpulseStrength = 2000.0f;
	RadialForce->bImpulseVelChange = true;
}

// Called when the game starts or when spawned
void AARLExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	MeshComponent->OnComponentHit.AddDynamic(this, &AARLExplosiveBarrel::OnComponentHit);
}

void AARLExplosiveBarrel::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForce->FireImpulse();
}

// Called every frame
void AARLExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

