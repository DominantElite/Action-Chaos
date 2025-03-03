// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickup.h"
#include "AmmoPickup.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONCHAOS_API AAmmoPickup : public ABasePickup
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float Ammo;
	virtual void HandlePickup(AActor* OtherActor, const FHitResult& SweepResult) override;
	virtual bool CanPickup(AActor* other) const override;
public:
	AAmmoPickup();
};
