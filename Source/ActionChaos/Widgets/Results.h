// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Results.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONCHAOS_API UResults : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButtonText* QuitButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButtonText* RestartButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UWidgetSwitcher* ResultsSwitcher;

	virtual void NativeConstruct() override;
public:
	void SetWin();
	void SetLoss();
};
