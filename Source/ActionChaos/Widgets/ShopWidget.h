// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Utility/InventoryInterface.h"
#include "ShopWidget.generated.h"


/**
 * 
 */
UCLASS()
class ACTIONCHAOS_API UShopWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UWidgetSwitcher* ShopSwitcher;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* Currency;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButtonText* WeaponsButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButtonText* ArmorButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButtonText* PurchaseButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButtonText* ExitButton;


	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UShopButtonWidget* StarterPistol;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int StartPistolCost;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UShopButtonWidget* Rifle;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int RifleCost;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UShopButtonWidget* Shotgun;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int ShotgunCost;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UShopButtonWidget* AutoPistol;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int AutoPistolCost;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UShopButtonWidget* AutoShotgun;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int AutoShotgunCost;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UShopButtonWidget* SemiRifle;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int SemiRifleCost;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UShopButtonWidget* BurstRifle;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int BurstRifleCost;



	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UShopButtonWidget* SpartanArmor;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int SPArmorCost;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButtonText* SpartanHelmet;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int SPHeadCost;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButtonText* SpartanChestplate;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int SPChestCost;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButtonText* SpartanGreaves;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int SPLegCost;


	TEnumAsByte<EShopItem::Item> CurrentlySelected;

	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
public:
	UFUNCTION(BlueprintCallable)
	void SetCurrency(int amount);
private:
	UFUNCTION()
	void WeaponsClicked();
	UFUNCTION()
	void PurchaseClicked();
	UFUNCTION()
	void ExitClicked();
	UFUNCTION()
	void ArmorClicked();

	UFUNCTION()
	void StarterPistolClicked();
	UFUNCTION()
	void RifleClicked();
	UFUNCTION()
	void ShotgunClicked();
	UFUNCTION()
	void AutoPistolClicked();
	UFUNCTION()
	void AutoShotgunClicked();
	UFUNCTION()
	void SemiRifleClicked();
	UFUNCTION()
	void BurstRifleClicked();

	UFUNCTION()
	void SpartanArmorClicked();
	UFUNCTION()
	void SpartanHelmetClicked();
	UFUNCTION()
	void SpartanChestplateClicked();
	UFUNCTION()
	void SpartanGreavesClicked();
};
