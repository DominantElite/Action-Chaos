// Fill out your copyright notice in the Description page of Project Settings.


#include "../AI/GetFlankCheck.h"
#include "AIController.h"
#include "../Actors/BaseEnemies.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UGetFlankCheck::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* OwController = OwnerComp.GetAIOwner();
	APawn* OwPawn = OwController->GetPawn();
	if (!OwPawn)
	{
		return EBTNodeResult::Failed;
	}
	IAIInterface* flank = Cast<IAIInterface>(OwPawn);
	if (flank)
	{
		flank->GetFlank();
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}
