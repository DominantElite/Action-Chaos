// Fill out your copyright notice in the Description page of Project Settings.


#include "../Actors/BaseArmor.h"
#include "../ActionChaos.h"

// Sets default values
ABaseArmor::ABaseArmor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	AllArmor = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
}

// Called when the game starts or when spawned
void ABaseArmor::BeginPlay()
{
	Super::BeginPlay();
	AActor* ActorArmor = this->GetParentActor();
	APawn* Pawn = Cast<APawn>(ActorArmor);
	if (Pawn)
	{
		ParentPawn = Pawn;
		if (pArmor == EArmorPiece::Head)
		{
			Head();
		}

		if (pArmor == EArmorPiece::Body)
		{
			if (Armor == EArmorType::Spartan)
			{
				CurrentShield = MaxShield;
				UE_LOG(Game, Warning, TEXT("I am being called dingus :p"));
			}
			Body();
		}

		if (pArmor == EArmorPiece::Legs)
		{
			//if (Armor == EArmorType::Spartan)
			//{

			//}
			Legs();
		}
	}
}

// Called every frame
void ABaseArmor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if ((CurrentShield < MaxShield) && pArmor == EArmorPiece::Body && Armor == EArmorType::Spartan)
	{
		if (canRechargeShields)
		{
			CurrentShield = FMath::FInterpConstantTo(CurrentShield, MaxShield, DeltaTime, RechargeTime);
		}
		OnShieldshit.Broadcast(CurrentShield / MaxShield);
	}
}

void ABaseArmor::Head()
{
	if (Armor == EArmorType::Spartan)
	{

	}
}

void ABaseArmor::Body()
{
	if (Armor == EArmorType::Spartan)
	{
		CurrentShield = MaxShield;
		UE_LOG(Game, Warning, TEXT("I am being called idiot :p"));
	}
}

void ABaseArmor::Legs()
{
	if (Armor == EArmorType::Spartan)
	{

	}
}

FName ABaseArmor::GetSocket()
{
	return ArmorSockets;
}

int ABaseArmor::GetCurrentShield()
{
	return CurrentShield;
}

void ABaseArmor::SetCurrentShield(int nShields)
{
	CurrentShield = nShields;
}

int ABaseArmor::GetMaxShield()
{
	return MaxShield;
}

float ABaseArmor::GetRechargeTime()
{
	return RechargeTime;
}

void ABaseArmor::RechargeShields()
{
	canRechargeShields = true;
}

float ABaseArmor::GetRechargeDelay()
{
	return RechargeDelay;
}

