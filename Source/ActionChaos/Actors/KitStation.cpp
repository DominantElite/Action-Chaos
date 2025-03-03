// Fill out your copyright notice in the Description page of Project Settings.


#include "KitStation.h"
#include "../ActionChaos.h"


// Sets default values
AKitStation::AKitStation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SpawnLocation = FVector(0.f, 0.f, 50.f);
}

// Called when the game starts or when spawned
void AKitStation::BeginPlay()
{
	Super::BeginPlay();
	StartSpawn(NULL);
}

void AKitStation::SpawnKit()
{
	int index = FMath::RandRange(0, KitTypes.Num() - 1);
	FActorSpawnParameters params;
	AActor* spawnedKit = GetWorld()->SpawnActor<AActor>(KitTypes[index], GetActorLocation() + SpawnLocation, FRotator::ZeroRotator, params);
	// for some reason the engine is allowed to execute the collision check and destruction before the literal next line of code
	if (spawnedKit)
		spawnedKit->OnDestroyed.AddDynamic(this, &AKitStation::StartSpawn);
	else {
		FTimerHandle spawning;
		GetWorld()->GetTimerManager().SetTimer(spawning, this, &AKitStation::SpawnKit, SpawnDelay * 2);
	}

}

// Called every frame
void AKitStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKitStation::StartSpawn(AActor* actor)
{
	FTimerHandle spawning;
	if (KitTypes.Num() >= 1)
		GetWorld()->GetTimerManager().SetTimer(spawning, this, &AKitStation::SpawnKit, SpawnDelay);
	else
		UE_LOG(Game, Error, TEXT("No pickups in array, so no spawning"));
}

