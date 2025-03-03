// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionChaosGameInstance.h"
#include "../Utility/SavedOptions.h"
#include <Kismet/GameplayStatics.h>

void UActionChaosGameInstance::LoadMainLevel()
{
	if (GameLevels.IsValidIndex(FirstLevelIndex))
	{
		CurrentLevelIndex = FirstLevelIndex;
		UGameplayStatics::OpenLevel(GetWorld(), GameLevels[FirstLevelIndex], true, FString());
	}
}

void UActionChaosGameInstance::QuitGame()
{
	APlayerController* TargetPC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	TargetPC->ConsoleCommand("quit");
}

void UActionChaosGameInstance::LoadCurrentLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), GameLevels[CurrentLevelIndex], true, FString());
}

void UActionChaosGameInstance::LoadMainMenu()
{
	CurrentLevelIndex = 0;
	UGameplayStatics::OpenLevel(GetWorld(), GameLevels[0], true, FString());
}

TArray<int> UActionChaosGameInstance::GetGameOptions()
{
	TArray<int> result;
	result.Add(Options->MapLength);
	result.Add(Options->MapWidth);
	result.Add(Options->Floors);
	result.Add(Options->MaxWaves);
	return result;
}
