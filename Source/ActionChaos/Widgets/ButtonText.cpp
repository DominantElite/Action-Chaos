// Fill out your copyright notice in the Description page of Project Settings.


#include "../Widgets/ButtonText.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

void UButtonText::NativePreConstruct()
{
	Super::NativePreConstruct();
	Information->SetText(Info);
}

void UButtonText::NativeConstruct()
{
	Super::NativeConstruct();
}

void UButtonText::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	BackgroundButton->OnClicked.AddDynamic(this, &UButtonText::HandleClickButton);

}

void UButtonText::HandleClickButton()
{
	OnButtonClicked.Broadcast();
}