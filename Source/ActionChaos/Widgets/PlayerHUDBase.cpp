// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDBase.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include <Kismet/GameplayStatics.h>
#include <Blueprint/SlateBlueprintLibrary.h>
#include "Components/TextBlock.h"
#include <ActionChaos/Actors/BaseEnemies.h>

void UPlayerHUDBase::NativeConstruct()
{
	Super::NativeConstruct();
	DynamicMaterial = Crosshair->GetDynamicMaterial();
	EndPoint = FVector::Zero();
}

void UPlayerHUDBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FGeometry cached = Crosshair->GetCachedGeometry();
	FVector2D pixelPos = FVector2D::ZeroVector;
	FVector2D viewportPos = FVector2D::ZeroVector;
	USlateBlueprintLibrary::LocalToViewport(this->GetWorld(), cached, FVector2D{ 0, 0 }, pixelPos, viewportPos);

	FVector worldPos = FVector::ZeroVector;
	FVector worldDir = FVector::ZeroVector;
	UGameplayStatics::DeprojectScreenToWorld(this->GetOwningPlayer(), cached.GetAbsoluteSize() * 0.5f + pixelPos, worldPos, worldDir);
	worldPos += worldDir * 100;

	FCollisionObjectQueryParams types;
	types.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	types.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);

	FHitResult hit;
	FVector end = worldPos + (worldDir * 100000);
	EndPoint = end;
	if (!GetWorld()->LineTraceSingleByObjectType(hit, worldPos, end, types))
		DynamicMaterial->SetVectorParameterValue(ColorName, SafeColor);
	else
	{
		RaycastHit = Cast<AActor>(hit.GetActor());
		ABaseEnemies* asEnemy = Cast<ABaseEnemies>(hit.GetActor());
		if (!asEnemy)
		{
			DynamicMaterial->SetVectorParameterValue(ColorName, SafeColor);
		}
		else
		{
			DynamicMaterial->SetVectorParameterValue(ColorName, DangerColor);
		}
		EndPoint = hit.ImpactPoint;
	}
}

void UPlayerHUDBase::SetHealth(float percent)
{
	HealthBar->SetPercent(percent);
}

void UPlayerHUDBase::SetShield(float percent)
{
	SheildBar->SetPercent(percent);
}

void UPlayerHUDBase::SetWeaponImage(TEnumAsByte<EShopItem::Item> weapon)
{
	switch (weapon)
	{
	case EShopItem::StartPistol:
		WeaponImage->SetBrushFromTexture(StartPistolImage);
		WeaponImage->SetDesiredSizeOverride(FVector2D(100, 100));
		break;
	case EShopItem::Rifle:
		WeaponImage->SetBrushFromTexture(RifleImage);
		WeaponImage->SetDesiredSizeOverride(FVector2D(217, 100));
		break;
	case EShopItem::Shotgun:
		WeaponImage->SetBrushFromTexture(ShotgunImage);
		WeaponImage->SetDesiredSizeOverride(FVector2D(200, 100));
		break;
	case EShopItem::AutoPistol:
		WeaponImage->SetBrushFromTexture(AutoPistolImage);
		WeaponImage->SetDesiredSizeOverride(FVector2D(100, 100));
		break;
	case EShopItem::AutoShotgun:
		WeaponImage->SetBrushFromTexture(AutoShotgunImage);
		WeaponImage->SetDesiredSizeOverride(FVector2D(500, 100));
		break;
	case EShopItem::SemiRifle:
		WeaponImage->SetBrushFromTexture(SemiRifleImage);
		WeaponImage->SetDesiredSizeOverride(FVector2D(330, 100));
		break;
	case EShopItem::BurstRifle:
		WeaponImage->SetBrushFromTexture(BurstRifleImage);
		WeaponImage->SetDesiredSizeOverride(FVector2D(433, 100));
		break;
	default:
		break;
	}
}

FVector UPlayerHUDBase::GetDestination() const
{
	return EndPoint;
}

AActor* UPlayerHUDBase::GetTarget() const
{
	return RaycastHit;
}

void UPlayerHUDBase::SetAmmo(float current, float max)
{
	CurrentAmmo->SetText(FText::FromString(FString::SanitizeFloat(current, 0)));
	MaxAmmo->SetText(FText::FromString(FString::SanitizeFloat(max, 0)));
}

void UPlayerHUDBase::SetWaves(int current, int max)
{
	CurrentWave->SetText(FText::FromString(FString::FromInt(current)));
	MaxWaves->SetText(FText::FromString(FString::FromInt(max)));
}

void UPlayerHUDBase::SetEnemyCount(int destroyed, int max)
{
	EnemyCount->SetText(FText::FromString(FString::FromInt(max - destroyed)));
}
