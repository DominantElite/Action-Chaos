// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopStation.h"
#include "ActionChaos/ActionChaosGameModeBase.h"
#include <ActionChaos/Widgets/ShopWidget.h>

// Sets default values
AShopStation::AShopStation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShopStation::BeginPlay()
{
	Super::BeginPlay();
	AActionChaosGameModeBase* gamemode = Cast<AActionChaosGameModeBase>(GetWorld()->GetAuthGameMode());
	if (gamemode && gamemode->CurrentController)
		ShopWidgetObject = Cast<UShopWidget>(CreateWidget(gamemode->CurrentController, ShopWidgetClass));
}

// Called every frame
void AShopStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShopStation::Interact()
{
	AActionChaosGameModeBase* gamemode = Cast<AActionChaosGameModeBase>(GetWorld()->GetAuthGameMode());
	if (gamemode) {
		if (!ShopWidgetObject) {
			if (gamemode->CurrentController) {
				ShopWidgetObject = Cast<UShopWidget>(CreateWidget(gamemode->CurrentController, ShopWidgetClass));
			}
		}
		if (ShopWidgetObject) {
			ShopWidgetObject->AddToViewport();
			FInputModeUIOnly InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
			InputMode.SetWidgetToFocus(ShopWidgetObject->TakeWidget());
			gamemode->CurrentController->SetInputMode(InputMode);
			gamemode->CurrentController->bShowMouseCursor = true;
		}
	}
}

