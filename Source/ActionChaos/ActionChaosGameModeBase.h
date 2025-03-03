// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ActionChaosGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyInfoVariable, AActor*, player);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCurrencyUpdateVariable, int, currency);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEnemyUpdateVariable, int, Destroyed, int, Max);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWaveChangedVariable, int, Wave, int, MaxWaves);


struct Structure
{
	FIntVector2 TopLeft;
	FIntVector2 BottomRight;
	int Layer;
	TArray<Structure*> Neighbors;
};

UENUM()
namespace EWorldGenVersion {
	enum Version {
		V1				UMETA(DisplayName = "Version 1"),
		V2				UMETA(DisplayName = "Version 2")
	};
}

/**
 * 
 */
UCLASS()
class ACTIONCHAOS_API AActionChaosGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AActionChaosGameModeBase();

	FEnemyInfoVariable OnPlayerSpotted;
	FWaveChangedVariable OnWaveChanged;
	FCurrencyUpdateVariable OnCurrencyChanged;
	FEnemyUpdateVariable OnEnemySpawned;

	/// map settings variables
	///////////////////////////////////////////////////
	int rows; //editable in options
	int columns; //editable in options
	int floors; //editable in options
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int maxAttempts;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int symmetryLines;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float lightChance;
	// this only matters with worldgen v1
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float stairChance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int minStructureWidth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int minStructureLength;
	// this only matters with worldgen v1
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int doorSetupVersion;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EWorldGenVersion::Version> worldGenVersion;
	///////////////////////////////////////////////////
	/// map settings variables

	/// game settings variables
	///////////////////////////////////////////////////
	int maxWaves; //editable in options
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float waveDelay;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float spawnRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int spawnCostMultiplier;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int spawnCostOffset;
	//make sure to match indexes with enemyCosts very closely
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<AActor>> enemyTypes; // would have loved enemyTypees [cost] [list of types for cost] but i cant have fun
	//make sure to match indexes with enemyTypes very closely
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<int> enemyCosts;
	//make sure to match indexes with enemyTypes very closely
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<int> enemyValues;
	///////////////////////////////////////////////////
	/// game settings variables

	void AddEnemy(AActor* enemyActor, int value);
	TArray<int> GetCurrentWave();
	void RemoveCurrency(int amount);
	TMap<AActor*, int> enemies;
	int currentWave;
	int playerCurrency;
	FVector enemySpawnpoint;
	FVector playerSpawnpoint;
	bool playerAlive;

	class ABasePlayer* currentPlayer;
	AActor* GetPlayerActor() const;
	APlayerController* CurrentController;

protected:
	virtual void BeginPlay() override;
	bool GenerateMap(int divisions);
	bool GenerateMapV2(int divisions);
	void SetupNextFloor(int divisions, TArray<TArray<int>>& field, int currFloor);
	void SetupMap(int divisions);
	void ClearMap();
	void NextWave();
	void SpawnEnemies();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> GridNodeClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> ShopStationClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> KitStationClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> PlayerClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> ResultsWidgetClass;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	class UResults* ResultsWidgetObject;
private:
	UFUNCTION()
	void RemoveEnemy(AActor* actor);
	UFUNCTION()
	void RemovePlayer(AActor* actor);
	TArray<TArray<TArray<int>>> map; //lets store it in x y z format in a special, automatically-resizing array
	int spawnPoints;
	TArray<int> enemySpawnList;
	int enemySpawnIndex;
	int enemiesKilled;
};
