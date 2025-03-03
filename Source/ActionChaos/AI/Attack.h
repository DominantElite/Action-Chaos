// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Attack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONCHAOS_API UAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Default)
	FName End;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Default)
	int bullets;
};
