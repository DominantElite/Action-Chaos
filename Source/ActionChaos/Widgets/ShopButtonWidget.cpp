// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopButtonWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UShopButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	Information->SetText(Info);
}

void UShopButtonWidget::NativeOnInitialized()
{
	Super::NativeConstruct();
	BackgroundButton->OnClicked.AddDynamic(this, &UShopButtonWidget::HandleClickButton);
}

void UShopButtonWidget::HandleClickButton()
{
	OnButtonClicked.Broadcast();
}