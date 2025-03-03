// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyEnemies.h"
#include <ActionChaos/ActionChaosGameModeBase.h>
#include <BehaviorTree/Blackboard/BlackboardKeyType_Object.h>
#include <BehaviorTree/BlackboardComponent.h>


UNotifyEnemies::UNotifyEnemies()
{
	PlayerKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UNotifyEnemies, PlayerKey), AActor::StaticClass());
}

EBTNodeResult::Type UNotifyEnemies::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* bb = OwnerComp.GetBlackboardComponent();
	AActionChaosGameModeBase* gm = Cast<AActionChaosGameModeBase>(GetWorld()->GetAuthGameMode());
	AActor* player = Cast<AActor>(bb->GetValue<UBlackboardKeyType_Object>(PlayerKey.GetSelectedKeyID()));
	if (gm && player)
	{
		gm->OnPlayerSpotted.Broadcast(player);
		return EBTNodeResult::Succeeded;
	}
	else
		return EBTNodeResult::Failed;
}


void UNotifyEnemies::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		PlayerKey.ResolveSelectedKey(*BBAsset);
	}
}

