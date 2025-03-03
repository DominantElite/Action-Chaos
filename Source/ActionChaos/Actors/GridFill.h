// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridFill.generated.h"

UCLASS()
class ACTIONCHAOS_API AGridFill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridFill();

protected:
	class UProceduralMeshComponent* ProceduralMesh;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void MakeBox(FVector3f dimensions);
	void MakeInvertedBox(FVector3f dimensions);
};
