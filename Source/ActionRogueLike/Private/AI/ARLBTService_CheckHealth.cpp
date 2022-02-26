// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARLBTService_CheckHealth.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "ARLAttributeComponent.h"

void UARLBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackBoardComp))
	{
		AAIController* MyController = OwnerComp.GetAIOwner();
		if (ensure(MyController))
		{
			APawn* AIPawn = MyController->GetPawn();
			if (ensure(AIPawn))
			{
				UARLAttributeComponent* AttributeComp = UARLAttributeComponent::GetAttributes(AIPawn);
					
				if (ensure(AttributeComp))
				{
					bool bIsHeathLow = AttributeComp->GetCurrentHealth() <= (AttributeComp->GetHealthMax() * LowHealthPercent);

					BlackBoardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bIsHeathLow);
				}
			}
		}
	}
}
