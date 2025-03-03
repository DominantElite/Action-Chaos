// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePickup.generated.h"

UCLASS(Abstract)
class ACTIONCHAOS_API ABasePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasePickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UBoxComponent* BoxCollision;
	virtual void HandlePickup(AActor* OtherActor, const FHitResult& SweepResult) PURE_VIRTUAL(ABasePickup::HandlePickup, );
	virtual void PostPickup();
	virtual bool CanPickup(AActor* other) const;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	UFUNCTION()
	void BoundOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
