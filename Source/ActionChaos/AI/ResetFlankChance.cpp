// Fill out your copyright notice in the Description page of Project Settings.


#include "../AI/ResetFlankChance.h"
#include "AIController.h"
#include "../Actors/BaseEnemies.h"

EBTNodeResult::Type UResetFlankChance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* OwnerController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = OwnerController->GetPawn();
	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}
	IAIInterface* noFlanky = Cast<IAIInterface>(ControlledPawn);
	if (noFlanky)
	{
		noFlanky->ResetFlank();
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
	
}
