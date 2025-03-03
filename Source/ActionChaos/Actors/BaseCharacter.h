// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Actors/BaseWeapon.h"
#include "../Actors/BaseArmor.h"
#include "../Utility/HealthComponent.h"
#include "../Utility/CharacterAnimation.h"
#include "BaseCharacter.generated.h"

UCLASS(Abstract)
class ACTIONCHAOS_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Weapon
	class UChildActorComponent* ChildActorComponent;

	// Armor
	class UChildActorComponent* HeadChildActorComponent;
	class UChildActorComponent* BodyChildActorComponent;
	class UChildActorComponent* LegsChildActorComponent;

	UFUNCTION()
	virtual void HandleHurt(float health);
	UFUNCTION()
	virtual void HandleShields(float shield);
	UFUNCTION()
	virtual void HandleDeathStart(float health);

	// Weapons and Weapon Class
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Variables|ClassReference")
	TSubclassOf<AActor> WeaponClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	ABaseWeapon* Weapon;

	// Armor Class and Armor
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Variables|ClassReference")
	TSubclassOf<AActor> ArmorHeadClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
	ABaseArmor* ArmorHead;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Variables|ClassReference")
	TSubclassOf<AActor> ArmorBodyClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
	ABaseArmor* ArmorBody;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Variables|ClassReference")
	TSubclassOf<AActor> ArmorLegsClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
	ABaseArmor* ArmorLegs;

	// Bools to determine stuff
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bisAI;

	// Health Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHealthComponent* HealthComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	UCharacterAnimation* CharacterAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Default)
	FName weaponSocketName;

	// Armor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Default)
	FName HeadSocketName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Default)
	FName BodySocketName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Default)
	FName LegsSocketName;

	UFUNCTION()
	void enableArmorPices();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// An AI Check
	bool IsAI();

private:
	UFUNCTION()
	void HandleRifleAttack();
};
