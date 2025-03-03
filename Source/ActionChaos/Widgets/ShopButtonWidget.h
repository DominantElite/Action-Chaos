// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopButtonWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonClickVariable);

/**
 * 
 */
UCLASS()
class ACTIONCHAOS_API UShopButtonWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* Information;
	UPROPERTY(meta = (BindWidget))
	class UButton* BackgroundButton;
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
private:
	UFUNCTION()
	void HandleClickButton();
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnButtonClickVariable OnButtonClicked;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Info;
};
