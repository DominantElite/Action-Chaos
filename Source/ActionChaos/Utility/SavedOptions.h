// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SavedOptions.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONCHAOS_API USavedOptions : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, SaveGame)
	int MapLength;
	UPROPERTY(BlueprintReadWrite, SaveGame)
	int MapWidth;
	UPROPERTY(BlueprintReadWrite, SaveGame)
	int Floors;
	UPROPERTY(BlueprintReadWrite, SaveGame)
	int MaxWaves;

	USavedOptions();
};
