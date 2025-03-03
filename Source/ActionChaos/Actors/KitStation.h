// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KitStation.generated.h"

UCLASS()
class ACTIONCHAOS_API AKitStation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKitStation();
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<AActor>> KitTypes;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector SpawnLocation;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float SpawnDelay;
private:
	void SpawnKit();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void StartSpawn(AActor* actor); // this only takes in an actor because i want it to activate a short time after the last kit was destroyed
};
