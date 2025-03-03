// Fill out your copyright notice in the Description page of Project Settings.


#include "../AI/FocusOnPlayer.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "../Actors/BaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Utility/CharacterAnimation.h"
#include "../ActionChaos.h"

EBTNodeResult::Type UFocusOnPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* OwController = OwnerComp.GetAIOwner();
	APawn* OwPawn = OwController->GetPawn();
	if (!OwPawn)
	{
		return EBTNodeResult::Failed;
	}
	UCharacterAnimation* Animation = Cast<UCharacterAnimation>((Cast<ACharacter>(OwPawn)->GetMesh())->GetAnimInstance());
	if (Animation)
	{
		//Animation->bIsPlayerSpotted = true;
		return EBTNodeResult::Succeeded;
	}
	else
	{
		UE_LOG(Game, Error, TEXT("There is no animation to grab"));
		return EBTNodeResult::Failed;
	}
}
