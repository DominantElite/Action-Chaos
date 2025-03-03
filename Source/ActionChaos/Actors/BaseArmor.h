// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseArmor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FArmorDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShields, float, Ratio);

UENUM(BlueprintType)
enum class EArmorType : uint8
{
	Spartan = 0 UMETA(DisplayName = "Spartan")
};

UENUM(BlueprintType)
enum class EArmorPiece : uint8
{
	Head = 0 UMETA(DisplayName = "Head"),
	Body = 1 UMETA(DisplayName = "Body"),
	Legs = 2 UMETA(DisplayName = "Legs")
};

UCLASS()
class ACTIONCHAOS_API ABaseArmor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseArmor();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EArmorType Armor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EArmorPiece pArmor;

	FShields OnShieldsDown;

	FShields OnShieldshit;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spartan")
	int MaxShield;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spartan")
	float RechargeTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spartan")
	float RechargeDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sockets")
	FName ArmorSockets;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class USkeletalMeshComponent* AllArmor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	TObjectPtr<APawn> ParentPawn;

	UFUNCTION()
	void Head();
	UFUNCTION()
	void Body();
	UFUNCTION()
	void Legs();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FName GetSocket();

	UFUNCTION()
	int GetCurrentShield();

	UFUNCTION()
	void SetCurrentShield(int nShields);

	UFUNCTION()
	int GetMaxShield();

	UFUNCTION()
	float GetRechargeTime();

	UFUNCTION()
	void RechargeShields();

	UFUNCTION()
	float GetRechargeDelay();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spartan")
	bool canRechargeShields;

private:
	UPROPERTY(VisibleAnywhere, Category = "Spartan", meta = (AllowPrivateAccess = "true"))
	float CurrentShield;
};
