// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Actors/BaseArmor.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthdelegate, float, health);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONCHAOS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void GetBodyArmor();

	int GetHealAmount(float percent);

	UFUNCTION()
	float GetCurrentHealth();

	UFUNCTION()
	float GetMaxHealth();

	UPROPERTY(BlueprintAssignable)
	FHealthdelegate OnHurt;

	UPROPERTY(BlueprintAssignable)
	FHealthdelegate OnDeath;

	UPROPERTY(BlueprintAssignable)
	FHealthdelegate OnHeal;

	// Temp
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Default)
	ABaseArmor* BodyArmor;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Max Health
	UPROPERTY(EditAnywhere, Category = Default)
	float MaxHealth;

private:	
	// Current Health
	UPROPERTY(VisibleAnywhere, Category = Default, meta = (AllowPrivateAccess = "true"))
	float CurrentHealth;

	// The Damage function
	UFUNCTION()
	void HandleDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

};
