// Fill out your copyright notice in the Description page of Project Settings.


#include "../AI/FindLoaction.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Actors/BaseEnemies.h"
#include "../ActionChaos.h"

EBTNodeResult::Type UFindLoaction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* OwController = OwnerComp.GetAIOwner();
	APawn* OwPawn = OwController->GetPawn();
	if (!OwPawn)
	{
		return EBTNodeResult::Failed;
	}
	UNavigationSystemV1* nav = FNavigationSystem::GetCurrent<UNavigationSystemV1>(OwPawn->GetWorld());
	FVector RandomLocation;
	if (nav->K2_GetRandomLocationInNavigableRadius(nav, OwPawn->GetActorLocation(), RandomLocation, Radius))
	{
		OwController->GetBlackboardComponent()->SetValueAsVector(UFindLoaction::LocationName, RandomLocation);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		UE_LOG(Game, Error, TEXT("No location was found in the world or no navigation mesh detected"));
		return EBTNodeResult::Failed;
	}
}
