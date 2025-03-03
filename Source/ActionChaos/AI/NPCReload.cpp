// Fill out your copyright notice in the Description page of Project Settings.


#include "../AI/NPCReload.h"
#include "AIController.h"
#include "../Actors/BaseEnemies.h"

EBTNodeResult::Type UNPCReload::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* OwnerController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = OwnerController->GetPawn();
	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}
	IAIInterface* iAgent = Cast<IAIInterface>(ControlledPawn);
	if (iAgent)
	{
		iAgent->Reload();
		OwnerComp.RegisterMessageObserver(this, EndingMessage);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}
