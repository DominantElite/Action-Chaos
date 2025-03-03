// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridNode.generated.h"

UCLASS()
class ACTIONCHAOS_API AGridNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridNode();
	void SetWall();
	void SetFloor(bool trimmed = false);
	void SetLightSource();
	// 0 = forward, 1 = right, 2 = back, 3 = left
	void SetLowStair(int direction);
	// 0 = forward, 1 = right, 2 = back, 3 = left
	void SetStair(int direction);
	// 0 = forward, 1 = right, 2 = back, 3 = left
	void SetHighStair(int direction);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent* Cube;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent* Plane;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class URectLightComponent* LightComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent* Stairs;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
