// Fill out your copyright notice in the Description page of Project Settings.


#include "../Widgets/Results.h"
#include <ActionChaos/Utility/ActionChaosGameInstance.h>
#include "ButtonText.h"
#include "Components/WidgetSwitcher.h"
#include "../ActionChaos.h"

void UResults::NativeConstruct()
{
	UActionChaosGameInstance* inst = Cast<UActionChaosGameInstance>(GetGameInstance());
	if (inst)
	{
		RestartButton->OnButtonClicked.AddDynamic(inst, &UActionChaosGameInstance::LoadMainLevel);
		QuitButton->OnButtonClicked.AddDynamic(inst, &UActionChaosGameInstance::LoadMainMenu);
	}
	else
		UE_LOG(Game, Error, TEXT("Game instance cannot be used for this class (wrong instance set?)"));
}

void UResults::SetWin()
{
	RestartButton->Info = FText::FromString("Replay");
	ResultsSwitcher->SetActiveWidgetIndex(1);
}

void UResults::SetLoss()
{
	RestartButton->Info = FText::FromString("Restart");
	ResultsSwitcher->SetActiveWidgetIndex(0);
}
