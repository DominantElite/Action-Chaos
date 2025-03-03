// Fill out your copyright notice in the Description page of Project Settings.


#include "../Actors/Projectile.h"
#include "Components/SphereComponent.h" 
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../ActionChaos.h"

// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// TESTING
	
	//SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>("SphereMesh");
	//SetRootComponent(SphereMesh);
	//SphereMesh->SetCollisionProfileName("NoCollision");
	//
	//SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	//SphereCollision->SetupAttachment(SphereMesh);


	// Set up the components
	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SetRootComponent(SphereCollision);

	// SphereMesh
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>("SphereMesh");
	SphereMesh->SetupAttachment(SphereCollision);
	SphereMesh->SetCollisionProfileName("NoCollision");

	// Projectile speed
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");

	// Projectile Collision 
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::HandleOverlap);

	// Projectile Mesh
	SphereMesh->SetStaticMesh(SphereMesh->GetStaticMesh());
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle timetoDestroy;
	GetWorld()->GetTimerManager().SetTimer(timetoDestroy, this, &AProjectile::K2_DestroyActor, DestroyTimer);

	// Controller
	AController* EventInstigator = Cast<AController>(this->GetOwner());
	if (EventInstigator)
	{
		OwnerController = EventInstigator;
	}
	else
	{
		UE_LOG(Game, Warning, TEXT("No Controller was set"));
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) // Fix the Projectile killing yourself bug :p
{
	if (this->GetInstigator() != OtherActor->GetInstigator())
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, OwnerController, this, UDamageType::StaticClass());
	}
	Destroy();
	UE_LOG(Game, Warning, TEXT("I hit something"));
}