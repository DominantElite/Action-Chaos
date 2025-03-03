// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopWidget.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "ButtonText.h"
#include "ShopButtonWidget.h"
#include "ActionChaos/ActionChaosGameModeBase.h"

void UShopWidget::NativeConstruct()
{
	Super::NativeConstruct();
	AActionChaosGameModeBase* gamemode = Cast<AActionChaosGameModeBase>(GetWorld()->GetAuthGameMode());
	if (gamemode)
		SetCurrency(gamemode->playerCurrency);

}

void UShopWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	AActionChaosGameModeBase* gamemode = Cast<AActionChaosGameModeBase>(GetWorld()->GetAuthGameMode());
	if (gamemode) {
		gamemode->OnCurrencyChanged.AddDynamic(this, &UShopWidget::SetCurrency);
	}
	WeaponsButton->OnButtonClicked.AddDynamic(this, &UShopWidget::WeaponsClicked);
	ArmorButton->OnButtonClicked.AddDynamic(this, &UShopWidget::ArmorClicked);
	PurchaseButton->OnButtonClicked.AddDynamic(this, &UShopWidget::PurchaseClicked);
	ExitButton->OnButtonClicked.AddDynamic(this, &UShopWidget::ExitClicked);

	StarterPistol->OnButtonClicked.AddDynamic(this, &UShopWidget::StarterPistolClicked);
	Rifle->OnButtonClicked.AddDynamic(this, &UShopWidget::RifleClicked);
	Shotgun->OnButtonClicked.AddDynamic(this, &UShopWidget::ShotgunClicked);
	AutoPistol->OnButtonClicked.AddDynamic(this, &UShopWidget::AutoPistolClicked);
	AutoShotgun->OnButtonClicked.AddDynamic(this, &UShopWidget::AutoShotgunClicked);
	SemiRifle->OnButtonClicked.AddDynamic(this, &UShopWidget::SemiRifleClicked);
	BurstRifle->OnButtonClicked.AddDynamic(this, &UShopWidget::BurstRifleClicked);

	SpartanArmor->OnButtonClicked.AddDynamic(this, &UShopWidget::SpartanArmorClicked);
	SpartanHelmet->OnButtonClicked.AddDynamic(this, &UShopWidget::SpartanHelmetClicked);
	SpartanChestplate->OnButtonClicked.AddDynamic(this, &UShopWidget::SpartanChestplateClicked);
	SpartanGreaves->OnButtonClicked.AddDynamic(this, &UShopWidget::SpartanGreavesClicked);
}

void UShopWidget::SetCurrency(int amount)
{
	Currency->SetText(FText::FromString(FString::FromInt(amount)));
}

void UShopWidget::WeaponsClicked()
{
	ShopSwitcher->SetActiveWidgetIndex(0);
	ArmorButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f));
	WeaponsButton->SetColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f));
}

void UShopWidget::PurchaseClicked()
{
	int cost = 0;
	switch (CurrentlySelected) {
	case EShopItem::None:
		return;
	case EShopItem::StartPistol:
		cost = StartPistolCost;
		break;
	case EShopItem::Rifle:
		cost = RifleCost;
		break;
	case EShopItem::Shotgun:
		cost = ShotgunCost;
		break;
	case EShopItem::AutoPistol:
		cost = AutoPistolCost;
		break;
	case EShopItem::AutoShotgun:
		cost = AutoShotgunCost;
		break;
	case EShopItem::SemiRifle:
		cost = SemiRifleCost;
		break;
	case EShopItem::BurstRifle:
		cost = BurstRifleCost;
		break;
	case EShopItem::SPArmor:
		cost = SPArmorCost;
		break;
	case EShopItem::SPHead:
		cost = SPHeadCost;
		break;
	case EShopItem::SPChest:
		cost = SPChestCost;
		break;
	case EShopItem::SPLeg:
		cost = SPLegCost;
		break;
	}
	AActionChaosGameModeBase* gamemode = Cast<AActionChaosGameModeBase>(GetWorld()->GetAuthGameMode());
	if (gamemode && gamemode->playerCurrency >= cost) {
		gamemode->RemoveCurrency(cost);
		IInventoryInterface* playerInv = Cast<IInventoryInterface>(gamemode->GetPlayerActor());
		switch (CurrentlySelected) {
		case EShopItem::StartPistol:
			playerInv->EquipWeapon(CurrentlySelected);
			break;
		case EShopItem::Rifle:
			playerInv->EquipWeapon(CurrentlySelected);
			break;
		case EShopItem::Shotgun:
			playerInv->EquipWeapon(CurrentlySelected);
			break;
		case EShopItem::AutoPistol:
			playerInv->EquipWeapon(CurrentlySelected);
			break;
		case EShopItem::AutoShotgun:
			playerInv->EquipWeapon(CurrentlySelected);
			break;
		case EShopItem::SemiRifle:
			playerInv->EquipWeapon(CurrentlySelected);
			break;
		case EShopItem::BurstRifle:
			playerInv->EquipWeapon(CurrentlySelected);
			break;
		case EShopItem::SPArmor:
			playerInv->EquipArmor(EShopItem::SPHead);
			playerInv->EquipArmor(EShopItem::SPChest);
			playerInv->EquipArmor(EShopItem::SPLeg);
			break;
		case EShopItem::SPHead:
			playerInv->EquipArmor(CurrentlySelected);
			break;
		case EShopItem::SPChest:
			playerInv->EquipArmor(CurrentlySelected);
			break;
		case EShopItem::SPLeg:
			playerInv->EquipArmor(CurrentlySelected);
			break;
		}
		CurrentlySelected = EShopItem::None;
	}
}

void UShopWidget::ExitClicked()
{
	/*WeaponsButton->OnButtonClicked.RemoveDynamic(this, &UShopWidget::WeaponsClicked);
	ArmorButton->OnButtonClicked.RemoveDynamic(this, &UShopWidget::ArmorClicked);
	PurchaseButton->OnButtonClicked.RemoveDynamic(this, &UShopWidget::PurchaseClicked);
	ExitButton->OnButtonClicked.RemoveDynamic(this, &UShopWidget::ExitClicked);

	StarterPistol->OnButtonClicked.RemoveDynamic(this, &UShopWidget::StarterPistolClicked);
	Rifle->OnButtonClicked.RemoveDynamic(this, &UShopWidget::RifleClicked);
	Shotgun->OnButtonClicked.RemoveDynamic(this, &UShopWidget::ShotgunClicked);

	SpartanArmor->OnButtonClicked.RemoveDynamic(this, &UShopWidget::SpartanArmorClicked);
	SpartanHelmet->OnButtonClicked.RemoveDynamic(this, &UShopWidget::SpartanHelmetClicked);
	SpartanChestplate->OnButtonClicked.RemoveDynamic(this, &UShopWidget::SpartanChestplateClicked);
	SpartanGreaves->OnButtonClicked.RemoveDynamic(this, &UShopWidget::SpartanGreavesClicked);*/
	AActionChaosGameModeBase* gamemode = Cast<AActionChaosGameModeBase>(GetWorld()->GetAuthGameMode());
	if (gamemode) {
		//gamemode->OnCurrencyChanged.RemoveDynamic(this, &UShopWidget::SetCurrency);
		if (gamemode->CurrentController && gamemode->playerAlive) {
			FInputModeGameOnly InputMode;
			gamemode->CurrentController->SetInputMode(InputMode);
			gamemode->CurrentController->bShowMouseCursor = false;
		}
	}
	this->RemoveFromParent();
}

void UShopWidget::ArmorClicked()
{
	ShopSwitcher->SetActiveWidgetIndex(1);
	WeaponsButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f));
	ArmorButton->SetColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f));
}

void UShopWidget::StarterPistolClicked()
{
	CurrentlySelected = EShopItem::StartPistol;
}

void UShopWidget::RifleClicked()
{
	CurrentlySelected = EShopItem::Rifle;
}

void UShopWidget::ShotgunClicked()
{
	CurrentlySelected = EShopItem::Shotgun;
}

void UShopWidget::AutoPistolClicked()
{
	CurrentlySelected = EShopItem::AutoPistol;
}

void UShopWidget::AutoShotgunClicked()
{
	CurrentlySelected = EShopItem::AutoShotgun;
}

void UShopWidget::SemiRifleClicked()
{
	CurrentlySelected = EShopItem::SemiRifle;
}

void UShopWidget::BurstRifleClicked()
{
	CurrentlySelected = EShopItem::BurstRifle;
}

void UShopWidget::SpartanArmorClicked()
{
	CurrentlySelected = EShopItem::SPArmor;
}

void UShopWidget::SpartanHelmetClicked()
{
	CurrentlySelected = EShopItem::SPHead;
}

void UShopWidget::SpartanChestplateClicked()
{
	CurrentlySelected = EShopItem::SPChest;
}

void UShopWidget::SpartanGreavesClicked()
{
	CurrentlySelected = EShopItem::SPLeg;
}
