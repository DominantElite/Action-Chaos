// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ActionChaosGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONCHAOS_API UActionChaosGameInstance : public UGameInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<FName> GameLevels = { "MainMenu", "TestingMap" };
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int FirstLevelIndex = 1;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USavedOptions* Options;
public:
	UFUNCTION()
	void LoadMainLevel();
	UFUNCTION()
	void QuitGame();
	UFUNCTION()
	void LoadCurrentLevel();
	UFUNCTION()
	void LoadMainMenu();
	UFUNCTION()
	TArray<int> GetGameOptions();
private:
	int CurrentLevelIndex = 0;
};
