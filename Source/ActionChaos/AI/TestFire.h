// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "TestFire.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONCHAOS_API UTestFire : public UBTService
{
	GENERATED_BODY()
	
public:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
