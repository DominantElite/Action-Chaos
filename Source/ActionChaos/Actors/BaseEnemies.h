// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Actors/BaseCharacter.h"
#include "../AI/AIInterface.h"
#include "BaseEnemies.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyDelegate);

/**
 * 
 */
UCLASS(Abstract)
class ACTIONCHAOS_API ABaseEnemies : public ABaseCharacter, public IAIInterface
{
	GENERATED_BODY()
	
public:
	ABaseEnemies();
	virtual void Tick(float DeltaTime) override;

	// Blackboard
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName HealthRatioName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName MovementTickName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName MoveItselfName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName FlankChanceName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName WalkChanceName;
	//

	// Medic target
	UFUNCTION(BlueprintCallable)
	void SetHealingTarget(ABaseEnemies* ComeHereSweetHeart); // Sets target Actor
	UFUNCTION(BlueprintCallable)
	FName GetHealingTargetName(); // Gets target actor name
	UFUNCTION(BlueprintCallable)
	void SetHealingTargetName(FName BlackboardHealingName); // Sets target actor name
	//

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Medic")
	bool bisMedic;

	// Aim Override
	FRotator GetBaseAimRotation() const override;
	//
	
	// Interface
	virtual void Attack() override;
	virtual void Reload() override;
	virtual void GetFlank() override;
	virtual void ResetFlank() override;
	virtual void Walk() override;
	virtual void Run() override;
	virtual void HealTarget() override;

	// HealthSTUFF
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Default)
	bool bisInjured;

	//FEnemyDelegate OnWalk;
	FEnemyDelegate OnFlank;

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName ActionFinishedMessage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName AmmoName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Medic")
	float HealPercentage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Medic")
	ABaseEnemies* HealingTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Medic")
	FName HealingTargetName;

	virtual void HandleHurt(float Ratio) override;
	UFUNCTION()
	void KillEnemy();
private:
	UFUNCTION()
	void UpdateBlackboardHealth(float Ratio);
	UFUNCTION()
	void UpdateBlackboardAmmo(float Current, float Max);
	UFUNCTION()
	void HandleActionFinished();
	UFUNCTION()
	void HandleMoveTime();
	FTimerHandle Reset;
};
