// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLInteractionComponent.h"
#include "ARLGameplayInterface.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UARLInteractionComponent::UARLInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UARLInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	
	AActor* MyOwner = GetOwner();


	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector EndPoint = EyeLocation + (EyeRotation.Vector() * 1000.0f);

	//FHitResult Hit;
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, EndPoint, ObjectQueryParams);

	TArray<FHitResult> Hits;

	float Radius = 30.0f;

	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, EndPoint, FQuat::Identity, ObjectQueryParams, Shape);
	
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	
	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<UARLGameplayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);

				IARLGameplayInterface::Execute_Interact(HitActor, MyPawn);
				break;
			}
		}
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f);
	}

	DrawDebugLine(GetWorld(), EyeLocation, EndPoint, LineColor, false, 2.0f, 0, 2.0f);
}

// Called when the game starts
void UARLInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UARLInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
