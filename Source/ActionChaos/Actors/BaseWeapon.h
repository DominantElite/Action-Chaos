// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRifleOnAmmoChanged, float, Current, float, Max);

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	pistol = 0 UMETA(DisplayName = "Pistol"),
	rifle = 1 UMETA(DisplayName = "Rifle"),
	shotgun = 2 UMETA(DisplayName = "Shotgun")
};

UCLASS(Abstract)
class ACTIONCHAOS_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadOnly) // weapon type
	EWeaponType wType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Neccessary components
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class USkeletalMeshComponent* AllWeapons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	TObjectPtr<APawn> ParentPawn;

	// Muzzle Socket
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sockets")
	FName MuzzleSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float ResetTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	bool Alive;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Default)
	bool Action;

	//Ammunition
	/////////////////////////////////////////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int MaxAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int AmmoinMag;
	/////////////////////////////////////////////////////////////////
	
	// Weapons modes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Modes")
	bool bIsAutomatic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Modes")
	bool bIsSemiAutomatic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Modes")
	bool bIsBurst;

	// FOR SHOTGUNS ONLY
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shotguns")
	bool bIsShotgun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shotguns")
	int numberOfPellets;

	// How many bullets will spawn when firing in burst mode
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	int BulletsPerBurst;

	// Universal Spread................... kinda :/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float spread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sockets")
	FName WeaponSocket;

	UFUNCTION()
	void UseAmmo();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsAutomatic() const;

	bool IsSemiAutomatic() const;

	bool IsShotgun() const;

	float GetTimer() const;

	FName GetSocket() const;

	void Shoot();

	void StopShooting();

	FVector GetSource();

	// Ammo Getters
	int GetCurrentAmmo();
	int GetMaxAmmo();
	//

	UFUNCTION()
	void OwnerDed();

	UFUNCTION()
	bool CanShoot() const;

	UFUNCTION(BlueprintCallable)
	void Reload();

	UFUNCTION(BlueprintCallable)
	void AIReload();

	void ReloadPercent(float percent);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Default)
	FWeaponDelegate OnActionStopped;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Default)
	FRifleOnAmmoChanged OnAmmoChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Default)
	FWeaponDelegate OnWeaponAttack;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Default)
	FWeaponDelegate OnReload;

private:
	bool bIsFiring = false;
	FTimerHandle Reset;
	void SemiStopShooting();
	int currentShot;
	// Ammunition
	int currentAmmo;

	// Saved ammo for refills
	int savedCurrAmmo;
	int savedMaxAmmo;
};
