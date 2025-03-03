// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"

UENUM()
namespace EShopItem {
	enum Item {
		None				UMETA(DisplayName = "Nothing Selected"),
		StartPistol			UMETA(DisplayName = "Starter Pistol"),
		Rifle				UMETA(DisplayName = "Rifle"),
		Shotgun				UMETA(DisplayName = "Shotgun"),
		AutoPistol			UMETA(DisplayName = "Auto Pistol"),
		AutoShotgun			UMETA(DisplayName = "Auto Shotgun"),
		SemiRifle			UMETA(DisplayName = "Semi Auto Rifle"),
		BurstRifle			UMETA(DisplayName = "Burst Rifle"),
		SPArmor				UMETA(DisplayName = "Spartan Armor (full set)"),
		SPHead				UMETA(DisplayName = "Spartan Armor Helmet"),
		SPChest				UMETA(DisplayName = "Spartan Armor Chestplate"),
		SPLeg				UMETA(DisplayName = "Spartan Armor Greaves")
	};
}

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ACTIONCHAOS_API IInventoryInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void EquipWeapon(TEnumAsByte<EShopItem::Item> weapon) = 0;
	virtual void EquipArmor(TEnumAsByte<EShopItem::Item> armor) = 0;
};
