// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimation.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAnimationVariable);


/**
 *
 */
UCLASS()
class ACTIONCHAOS_API UCharacterAnimation : public UAnimInstance
{
	GENERATED_BODY()
protected:
	// General Movement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Velocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Direction;



	// Mouse Movement
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Yaw;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Pitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAI;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector playerLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> FireAsset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> CurrentHitAsset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UAnimSequence>> HitAssets;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> ReloadAsset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> CurrentDeathAsset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UAnimSequence>> DeathAssets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ActionSlotName;

	UFUNCTION(BlueprintNativeEvent)
	void PreviewWindowUpdate();
	virtual void PreviewWindowUpdate_Implementation();
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsCrouching;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsPlayerSpotted;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsRunning;
	UFUNCTION(BlueprintCallable)
	void FireAnimation();
	UFUNCTION(BlueprintCallable)
	void HitAnimation();
	UFUNCTION(BlueprintCallable)
	void DeadAnimation();
	UFUNCTION(BlueprintCallable)
	void ReloadAnimation();
	UFUNCTION(BlueprintCallable)
	void FoundPlayer();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Default)
	FAnimationVariable OnActionEnded;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Default)
	FAnimationVariable OnReloadNow;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Default)
	FAnimationVariable OnDeathEnded;
	//UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Default)
	//FAnimationVariable OnLookAtPlayer;

private:
	UFUNCTION()
	void DeathEnded();
};
