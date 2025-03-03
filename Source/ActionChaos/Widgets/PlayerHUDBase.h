// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Utility/InventoryInterface.h"
#include "PlayerHUDBase.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONCHAOS_API UPlayerHUDBase : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar* HealthBar;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar* SheildBar;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage* Crosshair;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage* WeaponImage;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* CurrentAmmo;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* MaxAmmo;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* CurrentWave;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* EnemyCount;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* MaxWaves;
	UPROPERTY(BlueprintReadOnly)
	class UMaterialInstanceDynamic* DynamicMaterial;
	UPROPERTY(BlueprintReadWrite) // we are allowed to do this right? i forgot
	FLinearColor SafeColor;
	UPROPERTY(BlueprintReadWrite)
	FLinearColor DangerColor;
	UPROPERTY(BlueprintReadWrite)
	FName ColorName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UTexture2D* StartPistolImage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UTexture2D* RifleImage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UTexture2D* ShotgunImage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UTexture2D* AutoPistolImage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UTexture2D* AutoShotgunImage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UTexture2D* SemiRifleImage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UTexture2D* BurstRifleImage;
	FVector EndPoint;
	AActor* RaycastHit;
public:
	UFUNCTION(BlueprintCallable)
	virtual void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	UFUNCTION()
	void SetHealth(float percent);
	UFUNCTION()
	void SetShield(float percent);
	void SetWeaponImage(TEnumAsByte<EShopItem::Item> weapon);
	FVector GetDestination() const;
	AActor* GetTarget() const;
	UFUNCTION()
	void SetAmmo(float current, float max);
	UFUNCTION()
	void SetWaves(int current, int max);
	UFUNCTION()
	void SetEnemyCount(int destroyed, int max);
};
