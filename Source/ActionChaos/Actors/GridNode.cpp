// Fill out your copyright notice in the Description page of Project Settings.


#include "GridNode.h"
#include <Components/RectLightComponent.h>


// Sets default values
AGridNode::AGridNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Cube = CreateDefaultSubobject<UStaticMeshComponent>("Cube");
	SetRootComponent(Cube);

	Plane = CreateDefaultSubobject<UStaticMeshComponent>("Plane");
	Plane->SetupAttachment(Cube);
	Plane->AddRelativeLocation(FVector(0.f, 0.f, 249.5f));
	Plane->AddRelativeRotation(FRotator(-180.f, 0.f, 0.f));
	Plane->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Plane->SetVisibility(false);
	
	LightComponent = CreateDefaultSubobject<URectLightComponent>("LightComponent");
	LightComponent->bEditableWhenInherited = true;
	LightComponent->SetupAttachment(Plane);
	LightComponent->SetCastShadows(false);
	//Light->AddRelativeLocation(FVector(0.f, 0.f, 249.f));
	LightComponent->AddRelativeRotation(FRotator(90.f, 0.f, 0.f));
	LightComponent->SetVisibility(false);

	Stairs = CreateDefaultSubobject<UStaticMeshComponent>("Stairs");
	Stairs->SetupAttachment(Cube);
	Stairs->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
	Stairs->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Stairs->SetVisibility(false);
}

void AGridNode::SetWall()
{
	Cube->SetWorldScale3D(FVector(1.f, 1.f, 4.f));
	Cube->AddRelativeLocation(FVector(0.f, 0.f, 100.f));
	Cube->SetMobility(EComponentMobility::Static);
}

void AGridNode::SetFloor(bool trimmed)
{
	if (trimmed)
	{
		Cube->SetWorldScale3D(FVector(1.f, 1.f, 0.1f));
		Cube->AddRelativeLocation(FVector(0.f, 0.f, -5.f));
		Cube->SetMobility(EComponentMobility::Static);
	}
	else
	{
		Cube->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
		Cube->AddRelativeLocation(FVector(0.f, 0.f, -50.f));
		Cube->SetMobility(EComponentMobility::Static);
	}
}

void AGridNode::SetLightSource()
{
	Plane->SetVisibility(true, true);
	Plane->SetMobility(EComponentMobility::Stationary);
}

void AGridNode::SetLowStair(int direction)
{
	Cube->SetWorldScale3D(FVector(1.f, 1.f, 3.f));
	Cube->AddRelativeLocation(FVector(0.f, 0.f, -250.f));
	Cube->SetMobility(EComponentMobility::Static);
	Stairs->SetVisibility(true);
	Stairs->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Stairs->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
	Stairs->AddRelativeRotation(FRotator(0.f, 90.f * direction, 0.f));
	Stairs->AddRelativeLocation(FVector(0.f, 0.f, 50.f + 100.f / 6));
	Stairs->SetMobility(EComponentMobility::Static);
}

void AGridNode::SetStair(int direction)
{
	Stairs->SetVisibility(true);
	Stairs->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Stairs->AddRelativeRotation(FRotator(0.f, 90.f * direction, 0.f));
	Stairs->AddRelativeLocation(FVector(0.f, 0.f, 100.f));
	Stairs->SetMobility(EComponentMobility::Static);
	Cube->AddRelativeLocation(FVector(0.f, 0.f, -50.f));
	Cube->SetMobility(EComponentMobility::Static);
}

void AGridNode::SetHighStair(int direction)
{
	Cube->SetWorldScale3D(FVector(1.f, 1.f, 2.f));
	Cube->SetMobility(EComponentMobility::Static);
	Stairs->SetVisibility(true);
	Stairs->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Stairs->AddRelativeRotation(FRotator(0.f, 90.f * direction, 0.f));
	Stairs->AddRelativeLocation(FVector(0.f, 0.f, 75.f));
	Stairs->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
	Stairs->SetMobility(EComponentMobility::Static);
}

// Called when the game starts or when spawned
void AGridNode::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGridNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

