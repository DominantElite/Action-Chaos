// Fill out your copyright notice in the Description page of Project Settings.


#include "../Actors/BasicEnemy.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BaseEnemies.h"
#include "BasePlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <ActionChaos/ActionChaosGameModeBase.h>
#include "../ActionChaos.h"

ABasicEnemy::ABasicEnemy(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	BEPerception = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerception");
	BEConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("AIConfig_Sight");
	BEPerception->ConfigureSense(*BEConfig_Sight);

	BEPerception->OnPerceptionUpdated.AddDynamic(this, &ABasicEnemy::HandlePerception);
}

void ABasicEnemy::OnPossess(APawn* possesedPawen)
{
	Super::OnPossess(possesedPawen);
	AActionChaosGameModeBase* gm = Cast<AActionChaosGameModeBase>(GetWorld()->GetAuthGameMode());
	if (gm)
		gm->OnPlayerSpotted.AddDynamic(this, &ABasicEnemy::SetTarget);
	RunBehaviorTree(btAsset);
}

void ABasicEnemy::HandlePerception(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* Actor : UpdatedActors)
	{
		FActorPerceptionBlueprintInfo info;
		BEPerception->GetActorsPerception(Actor, info);
		for (FAIStimulus& stim : info.LastSensedStimuli)
		{
			Animations = Cast<UCharacterAnimation>(Cast<ABaseEnemies>(this->GetPawn())->GetMesh()->GetAnimInstance());
			if (!Animations)
			{
				UE_LOG(Game, Error, TEXT("No animations detected"));
			}
			if (Cast<ABasePlayer>(Actor))
			{
				if (stim.WasSuccessfullySensed())
				{
					SetTarget(Actor);
					Animations->bIsPlayerSpotted = true;
				}
				else
				{
					GetBlackboardComponent()->ClearValue(playerName);
					ClearFocus(EAIFocusPriority::Gameplay);
					Animations->bIsPlayerSpotted = false;
				}
			}

			// Medics
			ABaseEnemies* medic = Cast<ABaseEnemies>(this->GetPawn()); // Medic
			ABaseEnemies* injured = Cast<ABaseEnemies>(Actor); // The injured
			if (medic && injured && medic->bisMedic && injured->bisInjured)
			{
				if (stim.WasSuccessfullySensed())
				{
					GetBlackboardComponent()->SetValueAsObject(InjuredTargetName, injured);
					medic->SetHealingTarget(injured);
					medic->SetHealingTargetName(InjuredTargetName);
				}
				else
				{
					GetBlackboardComponent()->ClearValue(InjuredTargetName);
				}
			}
		}
	}
}

void ABasicEnemy::SetTarget(AActor* player)
{
	GetBlackboardComponent()->SetValueAsObject(playerName, player);
	SetFocus(player);
}