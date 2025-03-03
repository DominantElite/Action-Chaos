// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "../Utility/CharacterAnimation.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BasicEnemy.generated.h"

/**
 *  The AI Controller of the most basic enemies to fight
 */
UCLASS()
class ACTIONCHAOS_API ABasicEnemy : public AAIController
{
	GENERATED_BODY()
	
public:
	ABasicEnemy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void OnPossess(APawn* possesedPawen) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	TObjectPtr<UBehaviorTree> btAsset; // Behavior tree asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAIPerceptionComponent* BEPerception; // Basic Enemy Perception
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAISenseConfig_Sight* BEConfig_Sight; // Basic Enemy Sighting config
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	FName playerName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	FName InjuredTargetName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCharacterAnimation* Animations;
private:
	UFUNCTION()
	void HandlePerception(const TArray<AActor*>& UpdatedActors);
	UFUNCTION()
	void SetTarget(AActor* player);
};
