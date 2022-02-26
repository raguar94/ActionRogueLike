// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLInteractionComponent.h"
#include "ARLGameplayInterface.h"
#include "DrawDebugHelpers.h"
#include "ARLWorldUserWidget.h"


static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"), false, TEXT("Ënable Debug Lines for Interact Component."), ECVF_Cheat);

// Sets default values for this component's properties
UARLInteractionComponent::UARLInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TraceRadius = 30.f;
	TraceDistance = 500.0f;
	CollisionChannel = ECC_WorldDynamic;
	
}

// Called when the game starts
void UARLInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UARLInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindBestInteractable();

}

void UARLInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
        
    FCollisionObjectQueryParams ObjectQueryParams;
    ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);
    
    AActor* MyOwner = GetOwner();


    FVector EyeLocation;
    FRotator EyeRotation;
    MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

    FVector EndPoint = EyeLocation + (EyeRotation.Vector() * TraceDistance);

    //FHitResult Hit;
    //bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, EndPoint, ObjectQueryParams);

    TArray<FHitResult> Hits;

    FCollisionShape Shape;
    Shape.SetSphere(TraceRadius);

    bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, EndPoint, FQuat::Identity, ObjectQueryParams, Shape);
    
    FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	// clear before trying to fill
	FocusedActor = nullptr;
    
    for (FHitResult Hit : Hits)
    {
        if (bDebugDraw)
        {
        	DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false, 2.0f);
        }

        AActor* HitActor = Hit.GetActor();
        if (HitActor)
        {
        	if (HitActor->Implements<UARLGameplayInterface>())
        	{
        		FocusedActor = HitActor;
        		break;
        	}
        }
    }

	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<UARLWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;

			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}

    if (bDebugDraw)
    {
        DrawDebugLine(GetWorld(), EyeLocation, EndPoint, LineColor, false, 2.0f, 0, 2.0f);
    }
}


void UARLInteractionComponent::PrimaryInteract()
{
	if (FocusedActor == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to interact");
		return;;
	}
	
	APawn* MyPawn = Cast<APawn>(GetOwner());
	IARLGameplayInterface::Execute_Interact(FocusedActor, MyPawn);
}