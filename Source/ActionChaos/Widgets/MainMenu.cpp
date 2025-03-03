// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include <ActionChaos/Utility/ActionChaosGameInstance.h>
#include "ButtonText.h"
#include "../ActionChaos.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	UGameInstance* instance = GetGameInstance();
	if (instance)
	{
		UActionChaosGameInstance* inst = Cast<UActionChaosGameInstance>(instance);
		if (inst)
		{
			PlayButton->OnButtonClicked.AddDynamic(inst, &UActionChaosGameInstance::LoadMainLevel);
			QuitButton->OnButtonClicked.AddDynamic(inst, &UActionChaosGameInstance::QuitGame);
		}
		else
			UE_LOG(Game, Error, TEXT("We need code game instance"));
	}
	else
		UE_LOG(Game, Error, TEXT("We will never get past main menu"));
}