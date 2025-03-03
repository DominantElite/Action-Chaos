// Fill out your copyright notice in the Description page of Project Settings.


#include "../AI/Walk.h"
#include "AIController.h"
#include "../Actors/BaseEnemies.h"

EBTNodeResult::Type UWalk::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* OwController = OwnerComp.GetAIOwner();
	APawn* OwPawn = OwController->GetPawn();
	if (!OwPawn)
	{
		return EBTNodeResult::Failed;
	}
	IAIInterface* walk = Cast<IAIInterface>(OwPawn);
	if (walk)
	{
		walk->Walk();
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}
