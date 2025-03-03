// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"
#include <Engine/DamageEvents.h>
#include "Components/BoxComponent.h"
#include "../Utility/PickupInterface.h"
#include "../ActionChaos.h"


AHealthPickup::AHealthPickup()
{
	Healing = 5.f;
}

void AHealthPickup::HandlePickup(AActor* OtherActor, const FHitResult& SweepResult)
{
	//UGameplayStatics::ApplyDamage(OtherActor, -Healing, NULL, this, UDamageType::StaticClass());
	/*FDamageEvent damageEvent{};
	OtherActor->TakeDamage(-Healing, damageEvent, NULL, this);*/
	IPickupInterface* i = Cast<IPickupInterface>(OtherActor);
	if (i)
		i->RefillMaxHealth(Healing);
	else
		UE_LOG(Game, Error, TEXT("How did we get here..? (something without the interface tried to heal)"));
}

bool AHealthPickup::CanPickup(AActor* other) const
{
	IPickupInterface* i = Cast<IPickupInterface>(other);
	if (i)
		return i->CanPickupHealth();
	else
		return false;
}
