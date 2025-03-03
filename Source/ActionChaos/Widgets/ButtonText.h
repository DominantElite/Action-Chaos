// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ButtonText.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonClickedVariable);

/**
 * 
 */
UCLASS()
class ACTIONCHAOS_API UButtonText : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* Information;
	UPROPERTY(meta = (BindWidget))
	class UButton* BackgroundButton;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
private:
	UFUNCTION()
	void HandleClickButton();
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnButtonClickedVariable OnButtonClicked;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Info;
};
