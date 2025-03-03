// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FindLoaction.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONCHAOS_API UFindLoaction : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	FName LocationName;
};
