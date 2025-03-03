// Fill out your copyright notice in the Description page of Project Settings.


#include "../Actors/BaseEnemies.h"
#include "BrainComponent.h"
#include "AIController.h"
#include "../ActionChaosGameModeBase.h"
#include "../Utility/CharacterAnimation.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <Kismet/GameplayStatics.h>
#include "../ActionChaos.h"

ABaseEnemies::ABaseEnemies()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ABaseEnemies::BeginPlay()
{
	Super::BeginPlay();
	Weapon->OnActionStopped.AddDynamic(this, &ABaseEnemies::HandleActionFinished);
	UpdateBlackboardHealth(1.0f);
	Weapon->OnAmmoChanged.AddDynamic(this, &ABaseEnemies::UpdateBlackboardAmmo);

	UpdateBlackboardAmmo(Weapon->GetCurrentAmmo(), Weapon->GetMaxAmmo());

	CharacterAnimation->OnDeathEnded.AddDynamic(this, &ABaseEnemies::KillEnemy);
	HandleMoveTime();
	GetFlank();
}

void ABaseEnemies::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseEnemies::HandleHurt(float Ratio)
{
	Super::HandleHurt(Ratio);
	UpdateBlackboardHealth(Ratio);
	if (Ratio < .5f)
	{
		bisInjured = true;
	}
	else
	{
		bisInjured = false;
	}
}

void ABaseEnemies::SetHealingTarget(ABaseEnemies* ComeHereSweetHeart)
{
	HealingTarget = ComeHereSweetHeart;
}

FName ABaseEnemies::GetHealingTargetName()
{
	return HealingTargetName;
}

void ABaseEnemies::SetHealingTargetName(FName BlackboardHealingName)
{
	HealingTargetName = BlackboardHealingName;
}

FRotator ABaseEnemies::GetBaseAimRotation() const
{
	AActionChaosGameModeBase* Gamemode = Cast<AActionChaosGameModeBase>(GetWorld()->GetAuthGameMode());
	AAIController* AICont = Cast<AAIController>(GetController());
	if (Gamemode)
	{
		// Aim at the player
		return FRotationMatrix::MakeFromX(Gamemode->GetPlayerActor()->GetActorLocation() - Weapon->GetSource()).Rotator();
	}
	else
	{
		// Dont aim at the player
		return Super::GetBaseAimRotation();
	}
}

void ABaseEnemies::Attack()
{
	if (Weapon)
	{
		Weapon->Shoot();
	}
}

void ABaseEnemies::Reload()
{
	if (Weapon)
	{
		Weapon->AIReload();
	}
}

void ABaseEnemies::GetFlank()
{
	AAIController* AICont = Cast<AAIController>(GetController());
	if (AICont)
	{
		UBlackboardComponent* Blackboard = AICont->GetBlackboardComponent();
		if (Blackboard)
		{
			float randomNumber2Flank = FMath::FRandRange(.01f, 1.0f);
			Blackboard->SetValueAsFloat(FlankChanceName, randomNumber2Flank);
		}
		else
		{
			UE_LOG(Game, Error, TEXT("Blackboard wasn't found"));
		}
	}
	else
	{
		UE_LOG(Game, Error, TEXT("AIController wasn't found"));
	}
}

void ABaseEnemies::ResetFlank()
{
	AAIController* AICont = Cast<AAIController>(GetController());
	if (AICont)
	{
		UBlackboardComponent* Blackboard = AICont->GetBlackboardComponent();
		if (Blackboard)
		{
			Blackboard->SetValueAsFloat(FlankChanceName, 1.0f);
		}
		else
		{
			UE_LOG(Game, Error, TEXT("Blackboard wasn't found"));
		}
	}
	else
	{
		UE_LOG(Game, Error, TEXT("AIController wasn't found"));
	}
}

void ABaseEnemies::KillEnemy()
{
	if (GetMesh())
	{
		//GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		GetMesh()->SetSimulatePhysics(true);
		UE_LOG(Game, Warning, TEXT("I am a ragdoll"));
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	}
	else
	{
		UE_LOG(Game, Warning, TEXT("I am not a ragdoll"));
		Destroy();
	}

	//Destroy();
}

void ABaseEnemies::UpdateBlackboardHealth(float Ratio)
{
	AAIController* AICont = Cast<AAIController>(GetController());
	if (AICont)
	{
		UBlackboardComponent* Blackboard = AICont->GetBlackboardComponent();
		if (Blackboard)
		{
			Blackboard->SetValueAsFloat(HealthRatioName, Ratio);
		}
		else
		{
			UE_LOG(Game, Error, TEXT("Blackboard wasn't found"));
		}
	}
	else
	{
		UE_LOG(Game, Error, TEXT("AIController wasn't found"));
	}
}

void ABaseEnemies::UpdateBlackboardAmmo(float Current, float Max)
{
	AAIController* AICont = Cast<AAIController>(GetController());
	if (AICont)
	{
		UBlackboardComponent* Blackboard = AICont->GetBlackboardComponent();
		if (Blackboard)
		{
			Blackboard->SetValueAsFloat(AmmoName, Current);
		}
		else
		{
			UE_LOG(Game, Error, TEXT("Blackboard wasn't found"));
		}
	}
	else
	{
		UE_LOG(Game, Error, TEXT("AIController wasn't found"));
	}
}

void ABaseEnemies::HandleActionFinished()
{
	FAIMessage::Send(this, FAIMessage(ActionFinishedMessage, this, true));
}

// Interface
void ABaseEnemies::Walk()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void ABaseEnemies::Run()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}
//

// For Medic
void ABaseEnemies::HealTarget()
{
	UGameplayStatics::ApplyDamage(HealingTarget, -HealingTarget->HealthComp->GetHealAmount(HealPercentage), this->GetController(), this, UDamageType::StaticClass());
	HealingTarget->UpdateBlackboardHealth(HealingTarget->HealthComp->GetCurrentHealth());
	if (HealingTarget->HealthComp->GetCurrentHealth() >= HealingTarget->HealthComp->GetMaxHealth() / 2)
	{
		AAIController* AICont = Cast<AAIController>(GetController());
		if (AICont)
		{
			UBlackboardComponent* Blackboard = AICont->GetBlackboardComponent();
			if (Blackboard)
			{
				Blackboard->ClearValue(GetHealingTargetName());
				HealingTarget->bisInjured = false;
			}
			else
			{
				UE_LOG(Game, Error, TEXT("Blackboard wasn't found"));
			}
		}
		else
		{
			UE_LOG(Game, Error, TEXT("AIController wasn't found"));
		}
	}

}

void ABaseEnemies::HandleMoveTime()
{
	AAIController* AICont = Cast<AAIController>(GetController());
	if (AICont)
	{
		UBlackboardComponent* Blackboard = AICont->GetBlackboardComponent();
		if (Blackboard)
		{
			float randomNumber = FMath::FRandRange(.01f, 1.0f);
			float randomNumber2Walk = FMath::FRandRange(.01f, 1.0f);
			float randomNumber3Self = FMath::FRandRange(.01f, 1.0f);

			Blackboard->SetValueAsFloat(MovementTickName, randomNumber);
			Blackboard->SetValueAsFloat(WalkChanceName, randomNumber2Walk);
			Blackboard->SetValueAsFloat(MoveItselfName, randomNumber3Self);

			// Set a timer to repeat this function
			GetWorld()->GetTimerManager().SetTimer(Reset, this, &ABaseEnemies::HandleMoveTime, 3.5f, true);
		}
		else
		{
			UE_LOG(Game, Error, TEXT("Blackboard wasn't found"));
		}
	}
	else
	{
		UE_LOG(Game, Error, TEXT("AIController wasn't found"));
	}
}