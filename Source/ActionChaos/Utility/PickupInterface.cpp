// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupInterface.h"

// Add default functionality here for any IPickupInterface functions that are not pure virtual.

bool IPickupInterface::CanPickupHealth()
{
	return true;
}

bool IPickupInterface::CanPickupAmmo()
{
	return true;
}
