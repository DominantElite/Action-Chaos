// Fill out your copyright notice in the Description page of Project Settings.


#include "../AI/Run.h"
#include "AIController.h"
#include "../Actors/BaseEnemies.h"

EBTNodeResult::Type URun::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* OwController = OwnerComp.GetAIOwner();
	APawn* OwPawn = OwController->GetPawn();
	if (!OwPawn)
	{
		return EBTNodeResult::Failed;
	}
	IAIInterface* run = Cast<IAIInterface>(OwPawn);
	if (run)
	{
		run->Run();
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}
