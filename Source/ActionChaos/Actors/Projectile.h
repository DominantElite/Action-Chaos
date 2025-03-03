// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

#include "Projectile.generated.h"

UCLASS()
class ACTIONCHAOS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Creation of the Sphere Collision and mesh
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class USphereComponent* SphereCollision;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UStaticMeshComponent* SphereMesh;

	// Speed
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovement;

	// Damage
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Default)
	float Damage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Default)
	float DestroyTimer;

	// The Controller
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Default)
	TObjectPtr<AController> OwnerController;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
