// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARLBTTask_Heal.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "ARLAttributeComponent.h"

UARLBTTask_Heal::UARLBTTask_Heal()
{
	HealPercent = 1.f;
}

EBTNodeResult::Type UARLBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();

	if (ensure(MyController))
	{
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
		if (MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		UARLAttributeComponent* AttributeComp = UARLAttributeComponent::GetAttributes(MyPawn);

		if (AttributeComp == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		bool bHealApplyed = AttributeComp->ApplyHealthChange(MyPawn, AttributeComp->GetHealthMax() * HealPercent);

		return bHealApplyed ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;

	}

	return EBTNodeResult::Failed;
}

