// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "NotifyEnemies.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONCHAOS_API UNotifyEnemies : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UNotifyEnemies();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector PlayerKey;
};
