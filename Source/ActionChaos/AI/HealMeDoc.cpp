// Fill out your copyright notice in the Description page of Project Settings.


#include "../AI/HealMeDoc.h"
#include "AIController.h"
#include "../Actors/BaseEnemies.h"

EBTNodeResult::Type UHealMeDoc::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* OwController = OwnerComp.GetAIOwner();
	APawn* OwPawn = OwController->GetPawn();
	if (!OwPawn)
	{
		return EBTNodeResult::Failed;
	}
	IAIInterface* heal = Cast<IAIInterface>(OwPawn);
	if (heal)
	{
		heal->HealTarget();
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}
