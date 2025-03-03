// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Utility/InteractInterface.h"
#include "ShopStation.generated.h"

UCLASS()
class ACTIONCHAOS_API AShopStation : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShopStation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UUserWidget> ShopWidgetClass;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	class UUserWidget* ShopWidgetObject;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Inherited via IInteractInterface
	void Interact() override;

};
