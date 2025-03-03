// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "../Utility/PickupInterface.h"
#include "../Utility/InventoryInterface.h"
//#include <ActionChaos/Actors/BaseWeapon.h>
#include "BasePlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerLossVariable);

/**
 * 
 */
UCLASS()
class ACTIONCHAOS_API ABasePlayer : public ABaseCharacter, public IPickupInterface, public IInventoryInterface
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void HandleHurt(float health) override;
	virtual void HandleShields(float shield) override;
	virtual void HandleDeathStart(float health) override;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	class UCameraComponent* Camera;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Variables|ClassReference")
	TSubclassOf<UUserWidget> WidgetClass;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	class UUserWidget* WidgetObject;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<ABaseWeapon> StarterPistolClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<ABaseWeapon> RifleClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<ABaseWeapon> ShotgunClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<ABaseWeapon> AutoPistolClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<ABaseWeapon> AutoShotgunClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<ABaseWeapon> SemiRifleClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<ABaseWeapon> BurstRifleClass;
	

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<ABaseArmor> SpartanHelmetClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<ABaseArmor> SpartanChestplateClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<ABaseArmor> SpartanGreavesClass;

	APlayerController* playerController;
public:
	ABasePlayer();

	UFUNCTION()
	void RemoveSprint();

	FPlayerLossVariable OnPlayerLost;
	FPlayerLossVariable OnArmorChanged;

	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//FRotator GetBaseAimRotation() const override;
	void PlayerWin();
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//float speed;

	//FRotator GetBaseAimRotation() const override;
	virtual void EquipWeapon(TEnumAsByte<EShopItem::Item> weapon) override;
	virtual void EquipArmor(TEnumAsByte<EShopItem::Item> armor) override;

private:
	//bool bIsFiring = false;
	//FTimerHandle FireTimer;
	void Fire();
	void InputActionStopFiring();
	void InputAxisMoveForward(float AxisValue);
	void InputAxisStrafe(float AxisValue);


	void InputActionCrouch();
	void InputActionRunning();
	void InputActionReloadPressed();
	void InputActionInteractPressed();
	void InputActionJumpPressed();
	UFUNCTION()
	void PlayerLost();

	// Inherited via IPickupInterface
	void RefillMaxHealth(float percentage) override;
	void RefillMaxAmmo(float percentage) override;
};
