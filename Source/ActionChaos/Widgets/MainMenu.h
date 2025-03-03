// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONCHAOS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButtonText* PlayButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButtonText* OptionsButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButtonText* QuitButton;
	virtual void NativeConstruct() override;
};
