// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimation.h"
#include "../ActionChaos.h"
#include "ActionChaos/Actors/BasePlayer.h"
#include "ActionChaos/Actors/BaseEnemies.h"
#include <Kismet/GameplayStatics.h>

void UCharacterAnimation::PreviewWindowUpdate_Implementation()
{}

void UCharacterAnimation::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* pawn = TryGetPawnOwner();
	if (pawn)
	{
		FVector pawnVel = pawn->GetVelocity();
		FRotator pawnRot = pawn->GetActorRotation();
		Velocity = pawnVel.Length();

		Direction = CalculateDirection(pawnVel, pawnRot);

		// Aim Offset for the AI
		if (bIsAI)
		{
			if (bIsPlayerSpotted)
			{
				ABasePlayer* player = Cast<ABasePlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
				if (player)
				{
					FVector pLocation = player->GetActorLocation();
					ABaseEnemies* enemy = Cast<ABaseEnemies>(pawn);
					if (enemy)
					{
						FVector eLocation = enemy->GetActorLocation();
						FVector DirectionToPlayer = pLocation - eLocation;
						DirectionToPlayer.Normalize();
						FRotator LookAtRotation = DirectionToPlayer.Rotation();
						FRotator RelativeRotation = LookAtRotation - enemy->GetActorRotation();
						Yaw = FMath::Clamp(-RelativeRotation.Yaw, -90, 90);
						Pitch = FMath::Clamp(RelativeRotation.Pitch, -90, 90);
					}
				}
				else
				{
					UE_LOG(Game, Error, TEXT("Player was not found"));
				}
			}
		}
		else
		{
			FRotator playerAim = pawn->GetBaseAimRotation();

			// Pitch
			if (playerAim.Pitch >= 180.f)
			{
				Pitch = playerAim.Pitch - 360.f;
			}
			else
			{
				Pitch = playerAim.Pitch;
			}
			//Yaw
			//if (playerAim.Yaw >= 180.f)
			//{
			//	Yaw = playerAim.Yaw - 360.f;
			//}
			//else
			//{
			//	Yaw = playerAim.Yaw;
			//}
		}

	}
	else
	{
		PreviewWindowUpdate();
	}
}

void UCharacterAnimation::FireAnimation()
{
	PlaySlotAnimationAsDynamicMontage(FireAsset, ActionSlotName);
	UE_LOG(Game, Warning, TEXT("Shooting animations are present"));
}

void UCharacterAnimation::HitAnimation()
{
	int RandomIndex = FMath::RandRange(0, HitAssets.Num() - 1);
	if (RandomIndex >= 0 && RandomIndex < HitAssets.Num())
	{
		CurrentHitAsset = HitAssets[RandomIndex];
		PlaySlotAnimationAsDynamicMontage(CurrentHitAsset, ActionSlotName);
		UE_LOG(Game, Warning, TEXT("I am hit"));
	}
	else
	{
		UE_LOG(Game, Error, TEXT("Hit animation index out of range"));
	}
}

void UCharacterAnimation::DeadAnimation()
{
	CurrentDeathAsset = DeathAssets[FMath::RandRange(0, DeathAssets.Num() - 1)];
	PlaySlotAnimationAsDynamicMontage(CurrentDeathAsset, ActionSlotName);
	float time = CurrentDeathAsset->GetPlayLength();
	FTimerHandle dying;
	GetWorld()->GetTimerManager().SetTimer(dying, this, &UCharacterAnimation::DeathEnded, time);
}

void UCharacterAnimation::ReloadAnimation()
{
	PlaySlotAnimationAsDynamicMontage(ReloadAsset, ActionSlotName);
}

void UCharacterAnimation::FoundPlayer()
{
	// Temp
	//ABasePlayer* player = Cast<ABasePlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	//if (player)
	//{
	//	FVector pLocation = player->GetActorLocation();
	//	ABaseEnemies* enemy = Cast<ABaseEnemies>(pawn);
	//	if (enemy)
	//	{
	//		FVector eLocation = enemy->GetActorLocation();
	//		FVector DirectionToPlayer = pLocation - eLocation;
	//		DirectionToPlayer.Normalize();
	//		FRotator LookAtRotation = DirectionToPlayer.Rotation();
	//		FRotator RelativeRotation = LookAtRotation - enemy->GetActorRotation();
	//		Yaw = FMath::Clamp(RelativeRotation.Yaw, -180, 180);
	//		Pitch = FMath::Clamp(RelativeRotation.Pitch, -180, 180);
	//	}
	//}
	//else
	//{
	//	UE_LOG(Game, Error, TEXT(""))
	//}
}

void UCharacterAnimation::DeathEnded()
{
	OnDeathEnded.Broadcast();
}