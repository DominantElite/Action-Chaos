// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickup.h"
#include "HealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONCHAOS_API AHealthPickup : public ABasePickup
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float Healing;
	virtual void HandlePickup(AActor* OtherActor, const FHitResult& SweepResult) override;
	virtual bool CanPickup(AActor* other) const override;
public:
	AHealthPickup();
};
