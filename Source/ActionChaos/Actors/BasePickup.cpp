// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePickup.h"
#include "Components/BoxComponent.h"

// Sets default values
ABasePickup::ABasePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("Box Collision");
	SetRootComponent(BoxCollision);
}

// Called when the game starts or when spawned
void ABasePickup::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ABasePickup::BoundOverlap);
}

void ABasePickup::PostPickup()
{
	Destroy();
}

bool ABasePickup::CanPickup(AActor* other) const
{
	return true;
}

// Called every frame
void ABasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalRotation(FQuat(FVector::UpVector, 0.5 * DeltaTime));
}

void ABasePickup::BoundOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CanPickup(OtherActor))
	{
		HandlePickup(OtherActor, SweepResult);
		PostPickup();
	}
}

