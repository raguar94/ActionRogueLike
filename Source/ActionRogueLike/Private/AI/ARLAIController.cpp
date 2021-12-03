// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARLAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AARLAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign behaviorTree in yor AI Controller.")))
	{
		RunBehaviorTree(BehaviorTree);
	}
}
