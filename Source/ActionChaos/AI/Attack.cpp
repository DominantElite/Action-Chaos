// Fill out your copyright notice in the Description page of Project Settings.


#include "../AI/Attack.h"
//#include "NavigationSystem.h"
#include "../Actors/BaseEnemies.h"
#include"../Actors/BasePlayer.h"
#include "AIController.h"
#include <Kismet/GameplayStatics.h>
//#include "BehaviorTree/BlackboardComponent.h"
#include "../ActionChaos.h"

EBTNodeResult::Type UAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* OwController = OwnerComp.GetAIOwner();
	APawn* OwPawn = OwController->GetPawn();
	if (!OwPawn)
	{
		return EBTNodeResult::Failed;
	}
	IAIInterface* AIAttok = Cast<IAIInterface>(OwPawn);
	if (AIAttok)
	{
		AIAttok->Attack();
		OwnerComp.RegisterMessageObserver(this, End);
		return EBTNodeResult::InProgress;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}
