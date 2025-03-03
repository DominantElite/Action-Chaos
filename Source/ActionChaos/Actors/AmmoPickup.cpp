// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPickup.h"
#include <ActionChaos/Utility/PickupInterface.h>
#include "../ActionChaos.h"

AAmmoPickup::AAmmoPickup()
{
	Ammo = 0.5f;
}

void AAmmoPickup::HandlePickup(AActor* OtherActor, const FHitResult& SweepResult)
{
	IPickupInterface* i = Cast<IPickupInterface>(OtherActor);
	if (i)
		i->RefillMaxAmmo(Ammo);
	else
		UE_LOG(Game, Error, TEXT("How did we get here..? (something without the interface tried to heal)"));
}

bool AAmmoPickup::CanPickup(AActor* other) const
{
	IPickupInterface* i = Cast<IPickupInterface>(other);
	if (i)
		return i->CanPickupAmmo();
	else
		return false;
}

