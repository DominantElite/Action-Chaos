// Fill out your copyright notice in the Description page of Project Settings.


#include "../Utility/HealthComponent.h"
#include "../ActionChaos.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	//fN = CreateDefaultSubobject<ABaseArmor>
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	if (AActor* Owner = this->GetOwner())
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleDamage);

		//TArray<UChildActorComponent*> test;
		//Owner->GetComponents<UChildActorComponent>(test);
		//for (UChildActorComponent* tests : test)
		//{
		//	if (tests && tests->GetName() == TEXT("BodyChildActorComponent"))
		//	{
		//		ABaseArmor* arm = Cast<ABaseArmor>(tests->GetChildActor());
		//		if (arm)
		//		{
		//			BodyArmor = arm;
		//			break;
		//		}
		//		else
		//		{
		//			UE_LOG(Game, Error, TEXT("Something went wrong with the Casting of ABaseArmor"));
		//			break;
		//		}
		//	}
		//}
	}
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::GetBodyArmor()
{
	TArray<UChildActorComponent*> test;
	this->GetOwner()->GetComponents<UChildActorComponent>(test);
	for (UChildActorComponent* tests : test)
	{
		if (tests && tests->GetName() == TEXT("BodyChildActorComponent"))
		{
			ABaseArmor* arm = Cast<ABaseArmor>(tests->GetChildActor());
			if (arm)
			{
				BodyArmor = arm;
				break;
			}
			else
			{
				UE_LOG(Game, Error, TEXT("Something went wrong with the Casting of ABaseArmor"));
				break;
			}
		}
	}
}

int UHealthComponent::GetHealAmount(float percent)
{
	return MaxHealth * percent;
}

float UHealthComponent::GetCurrentHealth()
{
	return CurrentHealth;
}

float UHealthComponent::GetMaxHealth()
{
	return MaxHealth;
}

void UHealthComponent::HandleDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	int finalDamage = Damage;

	// Shields
	if (BodyArmor != nullptr && Damage > 0)
	{
		float curr = BodyArmor->GetCurrentShield();
		int ma = BodyArmor->GetMaxShield();
		UE_LOG(Game, Warning, TEXT("I got hit with shields"));
		if (curr > 0.)
		{
			if (curr - finalDamage >= 0.)
			{
				curr -= finalDamage;
				finalDamage = 0.f;
				BodyArmor->SetCurrentShield(curr);
				//BodyArmor->OnShieldshit.Broadcast(curr / ma);
			}
			else
			{
				finalDamage = fabs(curr - finalDamage);
				curr = 0.f;
				BodyArmor->SetCurrentShield(curr);
				//BodyArmor->OnShieldsDown.Broadcast(0.0f);

			}
		}
	}
	else
	{
		UE_LOG(Game, Warning, TEXT("The Armor is null"));
	}

	// Health
	CurrentHealth = FMath::Clamp(CurrentHealth - finalDamage, 0., MaxHealth);
	if (CurrentHealth > 0.)
	{
		if (finalDamage > 0.)
		{
			OnHurt.Broadcast(CurrentHealth / MaxHealth);
		}
		else
		{
			OnHeal.Broadcast(CurrentHealth / MaxHealth);
		}
	}
	else
	{
		if (AActor* SecondOwner = this->GetOwner())
		{
			SecondOwner->OnTakeAnyDamage.RemoveDynamic(this, &UHealthComponent::HandleDamage);
			OnDeath.Broadcast(0.0f);
		}
	}

	// Recharge check
	if (BodyArmor)
	{
		BodyArmor->canRechargeShields = false;
		FTimerHandle shieldstimehandler;
		GetWorld()->GetTimerManager().SetTimer(shieldstimehandler, BodyArmor, &ABaseArmor::RechargeShields, BodyArmor->GetRechargeDelay(), false);
	}
}

