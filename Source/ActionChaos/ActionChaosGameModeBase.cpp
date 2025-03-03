// Copyright Epic Games, Inc. All Rights Reserved.


#include "ActionChaosGameModeBase.h"
#include "ActionChaos.h"
#include "Actors/GridNode.h"
#include "Widgets/Results.h"
#include "Actors/BasePlayer.h"
#include "Engine.h"
#include "Actors/KitStation.h"
#include "Actors/ShopStation.h"
#include "Actors/BasePickup.h"
#include "Utility/ActionChaosGameInstance.h"


AActionChaosGameModeBase::AActionChaosGameModeBase()
{
	rows = 50;
	columns = 50;
	floors = 2;
	maxAttempts = 200;
	symmetryLines = 4;
	lightChance = 1.f;
	stairChance = 0.3f;
	currentWave = 0;
	playerCurrency = 0;
	playerAlive = true;
	maxWaves = 8;
	waveDelay = 10;
	spawnRate = 3;
	minStructureWidth = 5;
	minStructureLength = 5;
	spawnCostMultiplier = 4;
	spawnCostOffset = 1;
	doorSetupVersion = 0;
	worldGenVersion = EWorldGenVersion::V2;
}

AActor* AActionChaosGameModeBase::GetPlayerActor() const
{
	return currentPlayer;
}

void AActionChaosGameModeBase::BeginPlay()
{
	TArray<int> savedOptions = Cast<UActionChaosGameInstance>(GetGameInstance())->GetGameOptions();
	rows = savedOptions[0];
	columns = savedOptions[1];
	floors = savedOptions[2];
	maxWaves = savedOptions[3];
	NextWave();
	for (TActorIterator<ABasePlayer> itr(GetWorld()); itr; ++itr) {
		currentPlayer = *itr;
	}
	if (currentPlayer != nullptr) {
		currentPlayer->OnDestroyed.AddDynamic(this, &AActionChaosGameModeBase::RemovePlayer);
	}
	for (TActorIterator<APlayerController> itr(GetWorld(), PlayerControllerClass); itr; ++itr) {
		CurrentController = *itr;
		ResultsWidgetObject = Cast<UResults>(CreateWidget(CurrentController, ResultsWidgetClass));
		if (!ResultsWidgetObject)
			UE_LOG(Game, Error, TEXT("Results widget has an unexpected class... the game will crash when it ends"))
			break;
	}
}

/// map setup functions
///////////////////////////////////////////////////
bool AActionChaosGameModeBase::GenerateMap(int divisions)
{
	TArray<TArray<int>> field;
	int Xmax = rows;
	int Ymax = columns;
	if (divisions == 2)
	{
		Xmax = rows / 2;
		Ymax = columns;
	}
	else if (divisions == 4)
	{
		Xmax = rows / 2;
		Ymax = columns / 2;
	}
	field.SetNumZeroed(Xmax);
	// default first layer to all floor
	for (int i = 0; i < Xmax; i++) {
		field[i].SetNumZeroed(Ymax);
		for (int j = 0; j < Ymax; j++) {
			field[i][j] = 1;
		}
	}
	for (int l = 0; l <= floors; l++) {
		// generate rooms and place them in random spaces
		// for now, it will be only square rooms
		// mark things as empty, floor, wall, or occupied for actual map setup
		if (l != floors)
		{
			int combo = 0;
			for (int attempts = maxAttempts; attempts > 0;) {
				int freeX = Xmax;
				int freeY = Ymax;
				int width = FMath::RandRange(minStructureWidth, freeX);
				int length = FMath::RandRange(minStructureLength, freeY);
				FIntVector2 counter = { width, length }; //counts down from the size of the structure
				FIntVector2 location = { 0, 0 }; //changes every time it doesnt fit there
				bool fitted = false;
				while (location.X + width - 1 < Xmax && !fitted) {
					location.Y = 0;
					while (location.Y + length - 1 < Ymax && !fitted) {
						for (int x = location.X; x < location.X + width; x++) {
							for (int y = location.Y; y < location.Y + length; y++) {
								if (field[x][y] == 1 || //unoccupied floor
									((field[x][y] == 2 || field[x][y] == 3) && (counter.X == width || counter.Y == length || counter.X == 1 || counter.Y == 1))) //a wall on an edge
								{
									counter.Y--;
									if (counter.Y == 0) // success
										break;
								}
								else
									break;
							}
							if (counter.Y == 0) //success
							{
								counter.X--;
								if (counter.X == 0) //complete success
									break;
								counter.Y = length;
							}
							else
								break;
						}
						if (counter.X != 0 || counter.Y != 0) {
							location.Y++;
							counter = { width, length};
						}
						else {
							fitted = true;
						}
					}
					if (!fitted)
						location.X++;
				}
				if (fitted) { //actually set up structure
					freeX -= width * ((float)width / Xmax);
					freeY -= length * ((float)length / Ymax);
					bool door = false;
					bool NXdoor = false, PXdoor = false, NYdoor = false, PYdoor = false; //never have i had so much difficulty with coordinates
					for (int x = 0; x < width; x++) {
						for (int y = 0; y < length; y++) {
							if (field[x + location.X][y + location.Y] == 1 && x == width - 1 || y == length - 1 || x == 0 || y == 0) { //fill the edges with walls
								field[x + location.X][y + location.Y] = 2;
								if (!((x == width - 1 || x == 0) && (y == length - 1 || y == 0))) { // as long as it isnt a corner
									if (doorSetupVersion == 0 && (door && FMath::RandRange(1, 10) == 1) || (!door && FMath::RandRange(x + y, width + length - 3) == width + length - 3)) {  //try making a door
										if (x == width - 1) //these four if checks are to determine direction of door relative to structure
										{
											if (x + location.X != Xmax - 1) //the if checks inside are to prevent out of bounds errors
											{
												field[x + location.X + 1][y + location.Y + 1] = (field[x + location.X + 1][y + location.Y + 1] <= 3 ? 3 : field[x + location.X + 1][y + location.Y + 1]);
												field[x + location.X + 1][y + location.Y] = (field[x + location.X + 1][y + location.Y] <= 3 ? 3 : field[x + location.X + 1][y + location.Y]);
												field[x + location.X + 1][y + location.Y - 1] = (field[x + location.X + 1][y + location.Y - 1] <= 3 ? 3 : field[x + location.X + 1][y + location.Y - 1]);
												field[x + location.X][y + location.Y] = 3;
												door = true;
											}
										}
										else if (x == 0)
										{
											if (x + location.X != 0)
											{
												field[location.X - 1][y + location.Y + 1] = (field[location.X - 1][y + location.Y + 1] <= 3 ? 3 : field[location.X - 1][y + location.Y + 1]);
												field[location.X - 1][y + location.Y] = (field[location.X - 1][y + location.Y] <= 3 ? 3 : field[location.X - 1][y + location.Y]);
												field[location.X - 1][y + location.Y - 1] = (field[location.X - 1][y + location.Y - 1] <= 3 ? 3 : field[location.X - 1][y + location.Y - 1]);
												field[x + location.X][y + location.Y] = 3;
												door = true;
											}
										}
										else if (y == length - 1)
										{
											if (y + location.Y != Ymax - 1)
											{
												field[x + location.X - 1][y + location.Y + 1] = (field[x + location.X - 1][y + location.Y + 1] <= 3 ? 3 : field[x + location.X - 1][y + location.Y + 1]);
												field[x + location.X][y + location.Y + 1] = (field[x + location.X][y + location.Y + 1] <= 3 ? 3 : field[x + location.X][y + location.Y + 1]);
												field[x + location.X + 1][y + location.Y + 1] = (field[x + location.X + 1][y + location.Y + 1] <= 3 ? 3 : field[x + location.X + 1][y + location.Y + 1]);
												field[x + location.X][y + location.Y] = 3;
												door = true;
											}
										}
										else if (y == 0)
										{
											if (y + location.Y != 0)
											{
												field[x + location.X - 1][y + location.Y - 1] = (field[x + location.X - 1][y + location.Y - 1] <= 3 ? 3 : field[x + location.X - 1][y + location.Y - 1]);
												field[x + location.X][y + location.Y - 1] = (field[x + location.X][y + location.Y - 1] <= 3 ? 3 : field[x + location.X][y + location.Y - 1]);
												field[x + location.X + 1][y + location.Y - 1] = (field[x + location.X + 1][y + location.Y - 1] <= 3 ? 3 : field[x + location.X + 1][y + location.Y - 1]);
												field[x + location.X][y + location.Y] = 3;
												door = true;
											}
										}
									}
									if (doorSetupVersion == 1)
									{
										//these if checks are to determine direction of door relative to structure
										//and they see if both sides of the door is walkable first (important !!)
										//then they roll for door chance using different odds based on whether or not there is a door on that wall
										if ((x == width - 1 && x + location.X != Xmax - 1) && 
											((PXdoor && FMath::RandRange(1, 10) == 1) || (!PXdoor && FMath::RandRange(y, length - 3) == length - 3)) &&
											(field[x + location.X - 1][y + location.Y] == 1 || field[x + location.X - 1][y + location.Y] == 3) &&
											(field[x + location.X + 1][y + location.Y] == 1 || field[x + location.X + 1][y + location.Y] == 3)) 
										{
											field[x + location.X][y + location.Y] = 3;
											PXdoor = true;
										}
										else if ((x == 0 && x + location.X != 0) &&
											((NXdoor && FMath::RandRange(1, 10) == 1) || (!NXdoor && FMath::RandRange(y, length - 3) == length - 3)) &&
											(field[x + location.X - 1][y + location.Y] == 1 || field[x + location.X - 1][y + location.Y] == 3) &&
											(field[x + location.X + 1][y + location.Y] == 1 || field[x + location.X + 1][y + location.Y] == 3)) 
										{
											field[x + location.X][y + location.Y] = 3;
											NXdoor = true;
										}
										else if ((y == length - 1 && y + location.Y != Ymax - 1) &&
											((PYdoor && FMath::RandRange(1, 10) == 1) || (!PYdoor && FMath::RandRange(x, width - 3) == width - 3)) &&
											(field[x + location.X][y + location.Y - 1] == 1 || field[x + location.X][y + location.Y - 1] == 3) &&
											(field[x + location.X][y + location.Y + 1] == 1 || field[x + location.X][y + location.Y + 1] == 3))
										{
											field[x + location.X][y + location.Y] = 3;
											PYdoor = true;
											
										}
										else if ((y == 0 && y + location.Y != 0) &&
											((NYdoor && FMath::RandRange(1, 10) == 1) || (!NYdoor && FMath::RandRange(x, width - 3) == width - 3)) &&
											(field[x + location.X][y + location.Y - 1] == 1 || field[x + location.X][y + location.Y - 1] == 3) &&
											(field[x + location.X][y + location.Y + 1] == 1 || field[x + location.X][y + location.Y + 1] == 3))
										{
											field[x + location.X][y + location.Y] = 3;
											NYdoor = true;
										}
									}
								}
							}
							else //occupy floor
							{
								field[x + location.X][y + location.Y] = 3;
								// decide whether something should be a light or not (by checking if its neighbors are lights
								if (field[x + location.X][y + location.Y - 1] != 4 && 
									field[x + location.X][y + location.Y + 1] != 4 &&
									field[x + location.X - 1][y + location.Y] != 4 &&
									field[x + location.X + 1][y + location.Y] != 4 &&
									field[x + location.X + 1][y + location.Y + 1] != 4 &&
									field[x + location.X + 1][y + location.Y - 1] != 4 &&
									field[x + location.X - 1][y + location.Y + 1] != 4 &&
									field[x + location.X - 1][y + location.Y - 1] != 4)
								{
									if (FMath::RandRange(0.f, 1.f) <= lightChance) // roll for light
									{
										field[x + location.X][y + location.Y] = 4;
									}
								}
							}
						}
					}
					combo++;
					if (combo >= 5) //it seems success streaks are a little too strong, so they get a nerf
					{
						attempts--; //this also prevents stalling from oversights that cause overwriting
						combo = 0;
					}
				}
				else
				{
					//UE_LOG(Game, Warning, TEXT("Structure of %d width and %d length failed to spawn"), width, length);
					attempts--;
					combo = 0;
				}
			}
		}

		bool stairsPlaced = false;
		// go over the formation to decide stair placement
		if (l != floors){
			for (int i = 1; i < Xmax - 1; i++) {
				for (int j = 1; j < Ymax - 1; j++) {
					if (field[i][j] == 1 && FMath::RandRange(0.f, 1.f) <= stairChance) {
						if (field[i][j] == 1) {
							int open = 0;
							if (field[i - 1][j - 1] == 1)
								open++;
							if (field[i - 1][j + 1] == 1)
								open++;
							if (field[i + 1][j - 1] == 1)
								open++;
							if (field[i + 1][j + 1] == 1)
								open++;
							if (open > 1) {
								if (i < Xmax - 3 && field[i - 1][j] == 1 && field[i + 1][j] == 1 && field[i + 2][j] == 1) { //check right
									if (/*(map[i + 2][j + 1][k] == 2) ||*/
										((field[i + 3][j] == 2 || field[i + 3][j] == 3)) /*||
										(map[i + 2][j - 1][k] == 2)*/) {
										field[i][j] = 30;
										field[i + 1][j] = 31;
										field[i + 2][j] = 33;
										field[i + 3][j] = (field[i + 3][j] == 2) ? 5 : 6;
										stairsPlaced = true;
										break;
									}
								}
								else if (i > 2 && field[i + 1][j] == 1 && field[i - 1][j] == 1 && field[i - 2][j] == 1) { //then left
									if (/*(map[i - 2][j + 1][k] == 2) ||*/
										((field[i - 3][j] == 2 || field[i - 3][j] == 3)) /*||
										(map[i - 2][j - 1][k] == 2)*/) {
										field[i][j] = 10;
										field[i - 1][j] = 11;
										field[i - 2][j] = 13;
										field[i - 3][j] = (field[i - 3][j] == 2) ? 5 : 6;
										stairsPlaced = true;
										break;
									}
								}
								else if (j < Ymax - 3 && field[i][j - 1] == 1 && field[i][j + 1] == 1 && field[i][j + 2] == 1) { //then up
									if (/*(map[i - 1][j + 2][k] == 2) ||*/
										((field[i][j + 3] == 2 || field[i][j + 3] == 3)) /*||
										(map[i + 1][j + 2][k] == 2)*/) {
										field[i][j] = 40;
										field[i][j + 1] = 41;
										field[i][j + 2] = 43;
										field[i][j + 3] = (field[i][j + 3] == 2) ? 5 : 6;

										stairsPlaced = true;
										break;
									}
								}
								else if (j > 2 && field[i][j + 1] == 1 && field[i][j - 1] == 1 && field[i][j - 2] == 1) { //then down
									if (/*(map[i - 1][j - 2][k] == 2) ||*/
										((field[i][j - 3] == 2 || field[i][j - 3] == 3)) /*||
										(map[i + 1][j - 2][k] == 2)*/) {
										field[i][j] = 20;
										field[i][j - 1] = 21;
										field[i][j - 2] = 23;
										field[i][j - 3] = (field[i][j - 3] == 2) ? 5 : 6;
										stairsPlaced = true;
										break;
									}
								}
							}
						}
					}
				}
			}
			// if stair placement failed, try forcing it
			if (!stairsPlaced) {
				for (int i = 1; i < Xmax - 1; i++) {
					for (int j = 1; j < Ymax - 1; j++) {
						if (field[i][j] == 1) {
							int open = 0;
							if (field[i - 1][j - 1] == 1)
								open++;
							if (field[i - 1][j + 1] == 1)
								open++;
							if (field[i + 1][j - 1] == 1)
								open++;
							if (field[i + 1][j + 1] == 1)
								open++;
							if (open > 1) {
								if (i < Xmax - 3 && field[i - 1][j] == 1 && field[i + 1][j] == 1 && field[i + 2][j] == 1) { //check right
									if (/*(map[i + 2][j + 1][k] == 2) ||*/
										((field[i + 3][j] == 2 || field[i + 3][j] == 3)) /*||
										(map[i + 2][j - 1][k] == 2)*/) {
										field[i - 1][j] = 7;
										field[i][j] = 30;
										field[i + 1][j] = 31;
										field[i + 2][j] = 33;
										field[i + 3][j] = (field[i + 3][j] == 2) ? 5 : 6;
										stairsPlaced = true;
										break;
									}
								}
								else if (i > 2 && field[i + 1][j] == 1 && field[i - 1][j] == 1 && field[i - 2][j] == 1) { //then left
									if (/*(map[i - 2][j + 1][k] == 2) ||*/
										((field[i - 3][j] == 2 || field[i - 3][j] == 3)) /*||
										(map[i - 2][j - 1][k] == 2)*/) {
										field[i + 1][j] = 7;
										field[i][j] = 10;
										field[i - 1][j] = 11;
										field[i - 2][j] = 13;
										field[i - 3][j] = (field[i - 3][j] == 2) ? 5 : 6;
										stairsPlaced = true;
										break;
									}
								}
								else if (j < Ymax - 3 && field[i][j - 1] == 1 && field[i][j + 1] == 1 && field[i][j + 2] == 1) { //then up
									if (/*(map[i - 1][j + 2][k] == 2) ||*/
										((field[i][j + 3] == 2 || field[i][j + 3] == 3)) /*||
										(map[i + 1][j + 2][k] == 2)*/) {
										field[i][j - 1] = 7;
										field[i][j] = 40;
										field[i][j + 1] = 41;
										field[i][j + 2] = 43;
										field[i][j + 3] = (field[i][j + 3] == 2) ? 5 : 6;

										stairsPlaced = true;
										break;
									}
								}
								else if (j > 2 && field[i][j + 1] == 1 && field[i][j - 1] == 1 && field[i][j - 2] == 1) { //then down
									if (/*(map[i - 1][j - 2][k] == 2) ||*/
										((field[i][j - 3] == 2 || field[i][j - 3] == 3)) /*||
										(map[i + 1][j - 2][k] == 2)*/) {
										field[i][j + 1] = 7;
										field[i][j] = 20;
										field[i][j - 1] = 21;
										field[i][j - 2] = 23;
										field[i][j - 3] = (field[i][j - 3] == 2) ? 5 : 6;
										stairsPlaced = true;
										break;
									}
								}
							}
						}
					}
					if (stairsPlaced)
						break;
				}
			}
			if (!stairsPlaced)
			{
				UE_LOG(Game, Error, TEXT("No stairs placed for at least one floor"));
				return false;
			}
		}
		// right so the map has to be copied across the parallel lines before this continues
		// after first layer, generate a default amount of floor (ceilings) to connect walls
		SetupNextFloor(divisions, field, l);
		// with more limited ground, try everything again but with cheecking for empty spaces
	}
	return true;
}
bool AActionChaosGameModeBase::GenerateMapV2(int divisions)
{
	TArray<TArray<int>> field;
	TArray<Structure> boxes;
	int Xmax = rows;
	int Ymax = columns;
	if (divisions == 2)
	{
		Xmax = rows / 2;
		Ymax = columns;
	}
	else if (divisions == 4)
	{
		Xmax = rows / 2;
		Ymax = columns / 2;
	}
	field.SetNumZeroed(Xmax);
	// default first layer to all floor
	for (int i = 0; i < Xmax; i++) {
		field[i].SetNumZeroed(Ymax);
		for (int j = 0; j < Ymax; j++) {
			field[i][j] = 1;
		}
	}
	TArray<int> kitStationFloors;
	for (int i = 0; i < floors / divisions; i++) {
		int floor = 0;
		do {
			floor = FMath::RandRange(0, floors - 1);
		} while (kitStationFloors.Contains(floor) != false);
		kitStationFloors.Add(floor);
	}
	if (kitStationFloors.Num() == 0)
		kitStationFloors.Add(FMath::RandRange(0, floors - 1));

	for (int l = 0; l <= floors; l++) {
		if (l != floors)
		{
			TArray<Structure> layerBoxes;
			int combo = 0;
			FIntVector2 location = { 0, 0 }; //just to keep track of where we are
			//Structure* LastBox = 0;
			for (int attempts = maxAttempts; attempts > 0;) {
				int freeX = Xmax - location.X;
				int freeY = Ymax - location.Y;
				int width = FMath::RandRange(minStructureWidth, freeX);
				int length = FMath::RandRange(minStructureLength, freeY);
				FIntVector2 counter = { width, length }; //counts down from the size of the structure
				Structure CurrentBox;
				bool fitted = false;
				while (location.X + width < Xmax && !fitted) {
					location.Y = 0;
					while (location.Y + length < Ymax && !fitted) {
						CurrentBox.TopLeft = location;
						if (field[location.X][location.Y] == 1 || field[location.X][location.Y] == 2) {
							for (int x = location.X; x < location.X + width; x++) {
								for (int y = location.Y; y < location.Y + length; y++) {
									if (field[x][y] == 1 || //unoccupied floor
										(field[x][y] == 2  && (counter.X == width || counter.Y == length || counter.X == 1 || counter.Y == 1))) //a wall on an edge
									{
										counter.Y--;
										if (counter.Y == 0) // success
											break;
									}
									else
										break;
								}
								if (counter.Y == 0) //success
								{
									counter.X--;
									if (counter.X == 0) //complete success
										break;
									counter.Y = length;
								}
								else
									break;
							}
						}
						if (counter.X != 0 || counter.Y != 0) { //failed to get enough space
							location.Y++;
							if (length > minStructureLength + 1)
								length -= 1;
							counter = { width, length };
						}
						else {
							CurrentBox.BottomRight = FIntVector2(CurrentBox.TopLeft.X + width - 1, CurrentBox.TopLeft.Y + length - 1);
							/*if (LastBox != nullptr)
								LastBox->Neighbors.Add(&CurrentBox);*/
							//LastBox = &CurrentBox;
							fitted = true;
							if (Xmax - location.X < minStructureWidth)
								attempts = 0;
						}
					}
					if (!fitted)
					{
						location.X++;
					}
				}
				if (fitted) { //actually set up structure
					boxes.Add(CurrentBox);
					layerBoxes.Add(CurrentBox);
					for (int x = 0; x < width; x++) {
						for (int y = 0; y < length; y++) {
							if (field[x + location.X][y + location.Y] == 1 && x == width - 1 || y == length - 1 || x == 0 || y == 0) //fill the edges with walls
								field[x + location.X][y + location.Y] = 2;
							else //occupy floor
							{
								field[x + location.X][y + location.Y] = 3;
								// decide whether something should be a light or not (by checking if its neighbors are lights
								if (field[x + location.X][y + location.Y - 1] != 4 &&
									field[x + location.X][y + location.Y + 1] != 4 &&
									field[x + location.X - 1][y + location.Y] != 4 &&
									field[x + location.X + 1][y + location.Y] != 4 &&
									field[x + location.X + 1][y + location.Y + 1] != 4 &&
									field[x + location.X + 1][y + location.Y - 1] != 4 &&
									field[x + location.X - 1][y + location.Y + 1] != 4 &&
									field[x + location.X - 1][y + location.Y - 1] != 4)
								{
									if (FMath::RandRange(0.f, 1.f) <= lightChance) // roll for light
									{
										field[x + location.X][y + location.Y] = 4;
									}
								}
							}
						}
					}
					combo++;
					if (combo >= 5) //it seems success streaks are a little too strong, so they get a nerf
					{
						attempts--; //this also prevents stalling from oversights that cause overwriting
						combo = 0;
					}
				}
				else
				{
					//UE_LOG(Game, Warning, TEXT("Structure of %d width and %d length failed to spawn"), width, length);
					location.X = Xmax - freeX;
					location.Y = Ymax - freeY;
					attempts--;
					combo = 0;
				}
			}
			
			// i should be able to check for neighbors by looping through the entire array and checking 
			// if s2 has either 
			// x1=s1x2 with y1 or y2 between s1y1 and s1y2 or
			// y1=s1y2 with x1 or x2 between s1x1 and s1x2 with the max in each direction being reduced by 2
			// i dont ned to chck the top or left walls because ideally if a neighbor is found, then both thee source and target mark each other as neighbors
			// doors might not have to be stored in the structs, and instead placed during the checking for the neighbors and deecided by the smallest connected wall
			
			// ^^^^ walk of shame im just keeping those comments there
			// past me was basically wrong about everything except storing the doors and only needing to check 2 walls
			// we check of both directions are in bounds on both walls (so s2 has x1=s1x1 and y2 has to be larger than s1y1 and y1 has to be smaller than s1y2 and repeat with x and y flipped)
			// upon getting confirmation we set part of the location of the door with whichever axis isnt equal using a little bit of randomness, then place door and mutually add neighbor
			for (int i = 0; i < layerBoxes.Num(); i++) {
				Structure box = layerBoxes[i];
				// neighbor checks
				for (int j = 0; j < layerBoxes.Num(); j++) {
					Structure box2 = layerBoxes[j];
					if (i == j)
						continue;
					int doorX = box.BottomRight.X;
					int doorY = box.BottomRight.Y;
					bool neighbors = false;
					if (box.BottomRight.X == box2.TopLeft.X && box.BottomRight.Y - 1 > box2.TopLeft.Y && box.TopLeft.Y + 1 < box2.BottomRight.Y) {
						neighbors = true;
						doorY = FMath::RandRange((box.TopLeft.Y > box2.TopLeft.Y ? box.TopLeft.Y : box2.TopLeft.Y) + 1,
							(box.BottomRight.Y < box2.BottomRight.Y ? box.BottomRight.Y : box2.BottomRight.Y) - 1);
					}
					else if (box.BottomRight.Y == box2.TopLeft.Y && box.BottomRight.X - 1 > box2.TopLeft.X && box.TopLeft.X + 1 < box2.BottomRight.X) {
						neighbors = true;
						doorX = FMath::RandRange((box.TopLeft.X > box2.TopLeft.X ? box.TopLeft.X : box2.TopLeft.X) + 1,
							(box.BottomRight.X < box2.BottomRight.X ? box.BottomRight.X : box2.BottomRight.X) - 1);
					}
					if (neighbors) {
						field[doorX][doorY] = 3;
						box.Neighbors.Add(&box2);
						box2.Neighbors.Add(&box);
					}
				}
				// open space checks and filling
				if (box.Neighbors.Num() < 5 + ((box.BottomRight.X - box.TopLeft.X) / 10) + ((box.BottomRight.Y - box.TopLeft.Y) / 10)) { // just in case really large worlds are created, dynamic checks
					TArray<FIntVector3> areas; //[start] [end] [channel] beecause i dont feel like making four different for loops
					int indexChannel1 = -1;
					int indexChannel2 = -1;
					// top and bottom walls
					for (int x = box.TopLeft.X + 1; x < box.BottomRight.X; x++) {
						if (box.TopLeft.Y > 0 && (field[x][box.TopLeft.Y - 1] == 1 || field[x][box.TopLeft.Y - 1] == 7)) {
							if (indexChannel1 == -1) {
								areas.Add(FIntVector3(x, x, 1));
								indexChannel1 = areas.Num() - 1;
							}
							else
								areas[indexChannel1].Y = x;
						}
						else
							indexChannel1 = -1;
						if (box.BottomRight.Y < Ymax - 1 && (field[x][box.BottomRight.Y + 1] == 1 || field[x][box.BottomRight.Y + 1] == 7)) {
							if (indexChannel2 == -1) {
								areas.Add(FIntVector3(x, x, 2));
								indexChannel2 = areas.Num() - 1;
							}
							else
								areas[indexChannel2].Y = x;
						}
						else
							indexChannel2 = -1;
					}
					for (FIntVector3 line : areas)
					{
						int range = line.Y - line.X;
						if (range > 4) {
							// stairs against the wall
							int y;
							if (line.Z == 1)
								y = box.TopLeft.Y - 1;
							else
								y = box.BottomRight.Y + 1;
							int wallY = y + (line.Z == 1 ? 1 : -1);
							int ledge = FMath::RoundToInt((range - 3) / 2.f);
							int space = FMath::RoundToInt((range - 3) / 2.f);
							bool left = FMath::RandBool();
							if (left) { 
								// 10, 11, 13 (12)
								for (int j = 0; j < ledge - 1; j++)
									field[line.X + j][y] = 6;
								field[line.X + ledge - 1][y] = 6;
								field[line.X + ledge][y] = 13;
								field[line.X + ledge + 1][y] = 11;
								field[line.X + ledge + 2][y] = 10;
								for (int j = 0; j < space; j++)
									field[line.X + ledge + 3 + j][y] = 1;
								if (ledge > 1)
									field[FMath::RandRange(line.X, line.X + ledge - 1)][wallY] = 3;
								if (space > 1)
									field[FMath::RandRange(line.Y - space + 1, line.Y)][wallY] = 3;
								else
									field[line.Y][wallY] = 3;
							}
							else {
								// 30, 31, 33 (32)
								for (int j = 0; j < space; j++)
									field[line.X + j][y] = 1;
								field[line.X + space][y] = 30;
								field[line.X + space + 1][y] = 31;
								field[line.X + space + 2][y] = 33;
								field[line.X + space + 3][y] = 6;
								for (int j = 1; j < ledge; j++)
									field[line.X + space + 3 + j][y] = 6;
								if (space > 1)
									field[FMath::RandRange(line.X, line.X + space - 1)][wallY] = 3;
								else
									field[line.X][wallY] = 3;
								if (ledge > 1)
									field[FMath::RandRange(line.Y - ledge + 1, line.Y)][wallY] = 3;
							}
						}
						else if (line.Z == 1)
							field[FMath::RandRange(line.X, line.Y)][box.TopLeft.Y] = 3;
						else if (line.Z == 2)
							field[FMath::RandRange(line.X, line.Y)][box.BottomRight.Y] = 3;
					}
					// reset
					areas.Empty();
					indexChannel1 = -1;
					indexChannel2 = -1;
					// left and right walls
					for (int y = box.TopLeft.Y + 1; y < box.BottomRight.Y; y++) {
						if (box.TopLeft.X > 0 && (field[box.TopLeft.X - 1][y] == 1 || field[box.TopLeft.X - 1][y] == 7)) {
							if (indexChannel1 == -1) {
								areas.Add(FIntVector3(y, y, 1));
								indexChannel1 = areas.Num() - 1;
							}
							else
								areas[indexChannel1].Y = y;
						}
						else
							indexChannel1 = -1;
						if (box.BottomRight.X < Xmax - 1 && (field[box.BottomRight.X + 1][y] == 1 || field[box.BottomRight.X + 1][y] == 7)) {
							if (indexChannel2 == -1) {
								areas.Add(FIntVector3(y, y, 2));
								indexChannel2 = areas.Num() - 1;
							}
							else
								areas[indexChannel2].Y = y;
						}
						else
							indexChannel2 = -1;
					}
					for (FIntVector3 line : areas)
					{
						int range = line.Y - line.X;
						if (range > 4) {
							// stairs against the wall
							int x;
							if (line.Z == 1)
								x = box.TopLeft.X - 1;
							else
								x = box.BottomRight.X + 1;
							int wallX = x + (line.Z == 1 ? 1 : -1);
							int ledge = FMath::RoundToInt((range - 3) / 2.f);
							int space = FMath::RoundToInt((range - 3) / 2.f);
							bool up = FMath::RandBool();
							if (up) {
								//20, 21, 23 (22)
								for (int j = 0; j < ledge - 1; j++)
									field[x][line.X + j] = 6;
								field[x][line.X + ledge - 1] = 6;
								field[x][line.X + ledge] = 23;
								field[x][line.X + ledge + 1] = 21;
								field[x][line.X + ledge + 2] = 20;
								for (int j = 0; j < space; j++)
									field[x][line.X + ledge + j + 3] = 1;
								if (ledge > 1)
									field[wallX][FMath::RandRange(line.X, line.X + ledge - 1)] = 3;
								if (space > 1)
									field[wallX][FMath::RandRange(line.Y - space + 1, line.Y)] = 3;
								else
									field[wallX][line.Y] = 3;
							}
							else {
								//40, 41, 43 (42)
								for (int j = 0; j < space; j++)
									field[x][line.X + j] = 1;
								field[x][line.X + space] = 40;
								field[x][line.X + space + 1] = 41;
								field[x][line.X + space + 2] = 43;
								field[x][line.X + space + 3] = 6;
								for (int j = 1; j < ledge; j++)
									field[x][line.X + space + j + 3] = 6;
								if (space > 1)
									field[wallX][FMath::RandRange(line.X, line.X + space - 1)] = 3;
								else
									field[wallX][line.X] = 3;
								if (ledge > 1)
									field[wallX][FMath::RandRange(line.Y - ledge + 1, line.Y)] = 3;
							}
						}
						else if (line.Z == 1)
							field[box.TopLeft.X][FMath::RandRange(line.X, line.Y)] = 3;
						else if (line.Z == 2)
							field[box.BottomRight.X][FMath::RandRange(line.X, line.Y)] = 3;
					}
				}
				// stairs should spawn outside buildings that have no neighbors, with a chance to spawn outside buildings with few neighboars
				// eventually indoor stairs will be easy to add in this version of worldgen
			}
			if (kitStationFloors.Contains(l)) {
				if (layerBoxes.Num() > 0)
				{
					Structure box = layerBoxes[FMath::RandRange(0, layerBoxes.Num() - 1)];
					int x;
					int y;
					do {
						x = FMath::RandRange(box.TopLeft.X + 1, box.BottomRight.X - 1);
						y = FMath::RandRange(box.TopLeft.Y + 1, box.BottomRight.Y - 1);
					} while ((field[x + 1][y + 1] == 2 && // i <3 nasty if checks
								field[x + 1][y] == 3) ||
							(field[x - 1][y + 1] == 2 &&
								field[x][y + 1] == 3) ||
							(field[x + 1][y - 1] == 2 &&
								field[x][y - 1] == 3) ||
							(field[x - 1][y - 1] == 2 &&
								field[x - 1][y] == 3) ||
							(field[x + 1][y] == 2 &&
								field[x - 1][y] == 2) || 
							(field[x][y - 1] == 2 &&
								field[x][y + 1] == 2));
					field[x][y] = 50;
				}
				
			}
		}
		SetupNextFloor(divisions, field, l);
	}

	return true;
}
void AActionChaosGameModeBase::SetupNextFloor(int divisions, TArray<TArray<int>>& field, int currFloor)
{
	int l = currFloor; //ah... laziness...
	for (int x = 0; x < field.Num(); x++)
	{
		for (int y = 0; y < field[0].Num(); y++)
		{
			map[x][y][l] = field[x][y];
			if (divisions == 2) {
				switch (field[x][y]) {
				case 10:
					map[rows - 1 - x][y][l] = 30;
					break;
				case 11:
					map[rows - 1 - x][y][l] = 31;
					break;
				case 12:
					map[rows - 1 - x][y][l] = 32;
					break;
				case 13:
					map[x][y][l] = 0;
					map[rows - 1 - x][y][l] = 0;
					break;
				case 23:
					map[x][y][l] = 0;
					map[rows - 1 - x][y][l] = 0;
					break;
				case 30:
					map[rows - 1 - x][y][l] = 10;
					break;
				case 31:
					map[rows - 1 - x][y][l] = 11;
					break;
				case 32:
					map[rows - 1 - x][y][l] = 12;
					break;
				case 33:
					map[x][y][l] = 0;
					map[rows - 1 - x][y][l] = 0;
					break;
				case 43:
					map[x][y][l] = 0;
					map[rows - 1 - x][y][l] = 0;
					break;
				default:
					map[rows - 1 - x][y][l] = field[x][y];
					break;
				}
			}
			else if (divisions == 4) {
				map[x][y][l] = field[x][y];
				switch (field[x][y]) { // one of the many pitfalls of hardcoded ids is telling the game how to reflect things across axes
				case 10:
					map[rows - 1 - x][y][l] = 30;
					map[x][columns - 1 - y][l] = field[x][y];
					map[rows - 1 - x][columns - 1 - y][l] = 30;
					break;
				case 11:
					map[rows - 1 - x][y][l] = 31;
					map[x][columns - 1 - y][l] = field[x][y];
					map[rows - 1 - x][columns - 1 - y][l] = 31;
					break;
				case 12:
					map[rows - 1 - x][y][l] = 32;
					map[x][columns - 1 - y][l] = field[x][y];
					map[rows - 1 - x][columns - 1 - y][l] = 32;
					break;
				case 13:
					map[x][y][l] = 0;
					map[rows - 1 - x][y][l] = 0;
					map[x][columns - 1 - y][l] = 0;
					map[rows - 1 - x][columns - 1 - y][l] = 0;
					break;
				case 20:
					map[rows - 1 - x][y][l] = field[x][y];
					map[x][columns - 1 - y][l] = 40;
					map[rows - 1 - x][columns - 1 - y][l] = 40;
					break;
				case 21:
					map[rows - 1 - x][y][l] = field[x][y];
					map[x][columns - 1 - y][l] = 41;
					map[rows - 1 - x][columns - 1 - y][l] = 41;
					break;
				case 22:
					map[rows - 1 - x][y][l] = field[x][y];
					map[x][columns - 1 - y][l] = 42;
					map[rows - 1 - x][columns - 1 - y][l] = 42;
					break;
				case 23:
					map[x][y][l] = 0;
					map[rows - 1 - x][y][l] = 0;
					map[x][columns - 1 - y][l] = 0;
					map[rows - 1 - x][columns - 1 - y][l] = 0;
					break;
				case 30:
					map[rows - 1 - x][y][l] = 10;
					map[x][columns - 1 - y][l] = field[x][y];
					map[rows - 1 - x][columns - 1 - y][l] = 10;
					break;
				case 31:
					map[rows - 1 - x][y][l] = 11;
					map[x][columns - 1 - y][l] = field[x][y];
					map[rows - 1 - x][columns - 1 - y][l] = 11;
					break;
				case 32:
					map[rows - 1 - x][y][l] = 12;
					map[x][columns - 1 - y][l] = field[x][y];
					map[rows - 1 - x][columns - 1 - y][l] = 12;
					break;
				case 33:
					map[x][y][l] = 0;
					map[rows - 1 - x][y][l] = 0;
					map[x][columns - 1 - y][l] = 0;
					map[rows - 1 - x][columns - 1 - y][l] = 0;
					break;
				case 40:
					map[rows - 1 - x][y][l] = field[x][y];
					map[x][columns - 1 - y][l] = 20;
					map[rows - 1 - x][columns - 1 - y][l] = 20;
					break;
				case 41:
					map[rows - 1 - x][y][l] = field[x][y];
					map[x][columns - 1 - y][l] = 21;
					map[rows - 1 - x][columns - 1 - y][l] = 21;
					break;
				case 42:
					map[rows - 1 - x][y][l] = field[x][y];
					map[x][columns - 1 - y][l] = 22;
					map[rows - 1 - x][columns - 1 - y][l] = 22;
					break;
				case 43:
					map[x][y][l] = 0;
					map[rows - 1 - x][y][l] = 0;
					map[x][columns - 1 - y][l] = 0;
					map[rows - 1 - x][columns - 1 - y][l] = 0;
					break;
				default:
					map[rows - 1 - x][y][l] = field[x][y];
					map[rows - 1 - x][columns - 1 - y][l] = field[x][y];
					map[x][columns - 1 - y][l] = field[x][y];
					break;
				}
			}
			switch (field[x][y])
			{
			case 1: //downgrade floor to empty
				field[x][y] = 0;
				break;
			case 2: //downgrade wall to floor
				field[x][y] = 1;
				break;
			case 3: //sidegrade occupied floor to floor
				field[x][y] = 1;
				break;
			case 4: //sidegrade lit floor to floor
				field[x][y] = 1;
				break;
			case 5: // wall but banned from ceilings
				field[x][y] = 7;
				break;
			case 6: // floor with ceiling banned from ceilings
				field[x][y] = 7;
				break;
			case 7: // floor banned from ceiling
				field[x][y] = 0;
				break;
			case 13: // these next four are just placeehodlers for high stairs being turned into the actual stair
				field[x][y] = 12;
				break;
			case 23:
				field[x][y] = 22;
				break;
			case 33:
				field[x][y] = 32;
				break;
			case 43:
				field[x][y] = 42;
				break;
			case 50: // kit station spawns inside buildings
				field[x][y] = 1;
				break;
			default:
				field[x][y] = 0;
				break;
			}
		}
	}
}
void AActionChaosGameModeBase::SetupMap(int divisions)
{
	map.SetNumZeroed(rows);
	for (int i = 0; i < rows; i++) {
		map[i].SetNumZeroed(columns);
		for (int j = 0; j < columns; j++) {
			map[i][j].SetNumZeroed(floors + 1);
		}
	}
	int trueDivs = divisions;
	if (!(divisions == 1 || divisions == 2 || divisions == 4))
		trueDivs = 2;
	if (worldGenVersion == EWorldGenVersion::V1) {
		while (!GenerateMap(trueDivs)) {
			UE_LOG(Game, Error, TEXT("Map Generation failed. Re-generating..."));
		}
	}
	else if (worldGenVersion == EWorldGenVersion::V2) {
		while (!GenerateMapV2(trueDivs)) {
			UE_LOG(Game, Error, TEXT("Map Generation failed. Re-generating..."));
		}
	}

	//place shop station in level
	bool finished = false;
	while (!finished)
	{
		float floor = FMath::RandRange(0, floors - 1);
		for (int i = 0; i < rows; i++) {
			float x = i / 2;
			if ((i - 1) % 2 == 0)
				x = rows - 1 - i / 2;
			for (int j = 0; j < columns; j++) {
				float y = j / 2;
				if ((j - 1) % 2 == 0)
					y = columns - 1 - j / 2;
				if ((map[x][y][floor] == 1 || map[x][y][floor] == 3) && FMath::RandRange(0.f, 1.f) < 2 * ((float)i + j) / (rows + columns - 2)) {
					int walls = 0;
					for (int n = 0; n < 9; n++) {
						if ((x == 0 && n % 3 == 0) ||
							(y == 0 && n / 3 == 0) ||
							(x == rows - 1 && n % 3 == 2) ||
							(y == columns - 1 && n / 3 == 2))
							continue;
						if (map[x + (n % 3) - 1][y + (n / 3) - 1][floor] == 2)
							walls++;
					}
					if (walls % 2 != 0) {
						finished = true;
						map[x][y][floor] = 60;
						break;
					}
				}
			}
			if (finished)
				break;
		}
	}

	// now time to actually put things in place to make the map
	float Xoffset = rows * 50;
	float Yoffset = columns * 50;
	FActorSpawnParameters params;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			for (int k = 0; k <= floors; k++) {
				AGridNode* currentNode = GetWorld()->SpawnActor<AGridNode>(GridNodeClass, FVector(i * 100 - Xoffset, j * 100 - Yoffset, k * 300), FRotator::ZeroRotator, params);
				switch (map[i][j][k]) {
				case 0: // empty
					currentNode->Destroy();
					break;
				case 1: // floor
					currentNode->SetFloor();
					break;
				case 2: // wall
					currentNode->SetWall();
					if (map[i][j][k + 1] == 1) {
						map[i][j][k + 1] = 0;
					}
					break;
				case 3: // occupied floor (will have a ceiling)
					currentNode->SetFloor();
					break;
				case 4: // floor with ceiling light
					currentNode->SetFloor();
					currentNode->SetLightSource();
					break;
				case 5:
					currentNode->SetWall();
					break;
				case 6:
					currentNode->SetFloor();
					break;
				case 7:
					currentNode->SetFloor();
					break;
				case 10: // two digit format [stair direction + 1] [stair type]
					currentNode->SetStair(0);
					break;
				case 11:
					currentNode->SetHighStair(0);
					break;
				case 12:
					currentNode->SetLowStair(0);
					break;
				case 20:
					currentNode->SetStair(1);
					break;
				case 21:
					currentNode->SetHighStair(1);
					break;
				case 22:
					currentNode->SetLowStair(1);
					break;
				case 30:
					currentNode->SetStair(2);
					break;
				case 31:
					currentNode->SetHighStair(2);
					break;
				case 32:
					currentNode->SetLowStair(2);
					break;
				case 40:
					currentNode->SetStair(3);
					break;
				case 41:
					currentNode->SetHighStair(3);
					break;
				case 42:
					currentNode->SetLowStair(3);
					break;
				case 50: // kit station
					currentNode->SetFloor();
					GetWorld()->SpawnActor<AActor>(KitStationClass, FVector(i * 100 - Xoffset, j * 100 - Yoffset, k * 300), FRotator::ZeroRotator, params);
					break;
				case 60: // shop station
					currentNode->SetFloor();
					GetWorld()->SpawnActor<AActor>(ShopStationClass, FVector(i * 100 - Xoffset, j * 100 - Yoffset, k * 300 + 50), FRotator::ZeroRotator, params);
					break;
				}
			}
		}
	}

	// something to keep all pawns in the bounds
	// FVector(-100 - Xoffset, -100 - Yoffset, 0) to FVector(100 * rows - Xoffset, -100 - Yoffset, (floors + 2) * 300)
	// FVector(-100 - Xoffset, -100 - Yoffset, 0) to FVector(-100 - Xoffset, 100 * columns - Yoffset, (floors + 2) * 300)
	// FVector(100 * rows - Xoffset, -100 - Yoffset, 0) to FVector(100 * rows - Xoffset, 100 * columns - Yoffset, (floors + 2) * 300)
	// FVector((-100 - Xoffset, 100 * columns - Yoffset, 0) to FVector(100 * rows - Xoffset, 100 * columns - Yoffset, (floors + 2) * 300)

}
void AActionChaosGameModeBase::ClearMap()
{
	if (!map.IsEmpty())
	{
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				map[i][j].Empty();
			}
			map[i].Empty();
		}
		map.Empty();
	}
	for (TActorIterator<AGridNode> itr(GetWorld()); itr; ++itr) {
		AGridNode* Actor = *itr;
		Actor->Destroy();
	}
	for (TActorIterator<AKitStation> itr(GetWorld()); itr; ++itr) {
		AKitStation* Actor = *itr;
		Actor->Destroy();
	}
	for (TActorIterator<AShopStation> itr(GetWorld()); itr; ++itr) {
		AShopStation* Actor = *itr;
		Actor->Destroy();
	}
	for (TActorIterator<ABasePickup> itr(GetWorld()); itr; ++itr) {
		ABasePickup* Actor = *itr;
		Actor->Destroy();
	}
}
///////////////////////////////////////////////////
/// map setup functions

/// game management functions
///////////////////////////////////////////////////
void AActionChaosGameModeBase::AddEnemy(AActor* enemyActor, int value)
{
	enemies.Add(enemyActor, value);
	enemyActor->OnDestroyed.AddDynamic(this, &AActionChaosGameModeBase::RemoveEnemy);
	//we might need more functionality in here later so this function needs to be used
}
TArray<int> AActionChaosGameModeBase::GetCurrentWave()
{
	return enemySpawnList;
}
void AActionChaosGameModeBase::RemoveCurrency(int amount)
{
	if (amount <= playerCurrency)
		playerCurrency -= amount;
	else
		playerCurrency = 0;
	OnCurrencyChanged.Broadcast(playerCurrency);
}
void AActionChaosGameModeBase::RemoveEnemy(AActor* actor)
{
	playerCurrency += enemies[actor];
	OnCurrencyChanged.Broadcast(playerCurrency);
	enemies.Remove(actor);
	enemiesKilled++;
	OnEnemySpawned.Broadcast(enemiesKilled, enemySpawnList.Num());
	if (enemySpawnList.Num() - enemiesKilled <= 0) { // game win !!
		UE_LOG(Game, Warning, TEXT("You win!! (this wave)"));
 		if (currentWave == maxWaves) {
			UE_LOG(Game, Error, TEXT("You win the game !!!!!!"));
			ResultsWidgetObject->AddToViewport();
			ResultsWidgetObject->SetWin();
			FInputModeUIOnly InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
			InputMode.SetWidgetToFocus(ResultsWidgetObject->TakeWidget());
			CurrentController->SetInputMode(InputMode);
			currentPlayer->PlayerWin();
			CurrentController->bShowMouseCursor = true;
		}
		else
			NextWave();

	}
	else { // notify things that need to be notified of the removal of an enemy (like the enemy count required for wave based gameplay)
		
		UE_LOG(Game, Warning, TEXT("%d enemies remaining"), enemies.Num());

	}
}
void AActionChaosGameModeBase::RemovePlayer(AActor* actor)
{
	UE_LOG(Game, Warning, TEXT("You lose!!"));
	ResultsWidgetObject->AddToViewport();
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	InputMode.SetWidgetToFocus(ResultsWidgetObject->TakeWidget());
	CurrentController->SetInputMode(InputMode);
	CurrentController->bShowMouseCursor = true;
	playerAlive = false;
}
void AActionChaosGameModeBase::NextWave()
{
	playerAlive = true;
	enemySpawnIndex = 0;
	enemySpawnList.Empty();
	enemiesKilled = 0;
	currentWave++;
	OnWaveChanged.Broadcast(currentWave, maxWaves);

	ClearMap();
	SetupMap(symmetryLines);

	bool finished = false;
	//find shomewhere to spawn the player
	while (!finished)
	{
		float playerSpawnFloor = FMath::RandRange(0, floors);
		for (int i = 0; i < rows; i++) {
			float x = rows / 2 - 1 + i / 2;
			if ((i - 1) % 2 == 0)
				x = rows - 1 / 2 - 1 - i / 2;
			for (int j = 0; j < columns; j++) {
				float y = columns / 2 - 1 + j / 2;
				if ((j - 1) % 2 == 0)
					y = columns / 1 - 1 - j / 2;
				if ((map[x][y][playerSpawnFloor] == 1 || map[x][y][playerSpawnFloor] == 3) && FMath::RandRange(0.f, 1.f) < 2 * ((float)i + j) / (rows + columns - 2)) {
					finished = true;
					playerSpawnpoint = { x * 100 - (rows) * 50, y * 100 - (columns) * 50, playerSpawnFloor * 300 + 90 };
					break;
				}
			}
			if (finished)
				break;
		}
	}
	finished = false;
	//find somewhere enemies should spawn from
	while (!finished)
	{
		float enemySpawnFloor = FMath::RandRange(0, floors);
		for (int i = 0; i < rows; i++) {
			float x = i / 2;
			if ((i - 1) % 2 == 0)
				x = rows - 1 - i / 2;
			for (int j = 0; j < columns; j++) {
				float y = j / 2;
				if ((j - 1) % 2 == 0)
					y = columns - 1 - j / 2;
				if ((map[x][y][enemySpawnFloor] == 1 || map[x][y][enemySpawnFloor] == 3) && FMath::RandRange(0.f, 1.f) < 2 * ((float)i + j) / (rows + columns - 2)) {
					finished = true;
					enemySpawnpoint = { x * 100 - (rows) * 50, y * 100 - (columns) * 50, enemySpawnFloor * 300 + 90 };
					break;
				}
			}
			if (finished)
				break;
		}
	}
	if (currentPlayer == nullptr) {
		for (TActorIterator<ABasePlayer> itr(GetWorld()); itr; ++itr) {
			currentPlayer = *itr;
		}
		if (currentPlayer == nullptr) {
			FActorSpawnParameters params;
			currentPlayer = GetWorld()->SpawnActor<ABasePlayer>(PlayerClass, playerSpawnpoint, FRotator::ZeroRotator, params);
			currentPlayer->OnDestroyed.AddDynamic(this, &AActionChaosGameModeBase::RemovePlayer);
		}
		else
			currentPlayer->SetActorLocation(playerSpawnpoint);
	}
	else
		currentPlayer->SetActorLocation(playerSpawnpoint);

	// generate wave enemies
	spawnPoints = spawnCostOffset + spawnCostMultiplier * currentWave;
	while (spawnPoints > 0)
	{
		TArray<int> affordable;
		for (int i = 0; i < enemyCosts.Num(); i++) {
			if (spawnPoints >= enemyCosts[i])
				affordable.Add(i);
		}
		if (affordable.Num() > 0)
		{
			int index = FMath::RandRange(0, affordable.Num() - 1);
			enemySpawnList.Add(index);
			spawnPoints -= enemyCosts[index];
		}
		else {
			UE_LOG(Game, Warning, TEXT("Nothing was cheap enough to add to spawn list with %d points remaining"), spawnPoints);
			break;
		}
	}
	OnEnemySpawned.Broadcast(0, enemySpawnList.Num());

	FTimerHandle spawnTimer;
	GetWorld()->GetTimerManager().SetTimer(spawnTimer, this, &AActionChaosGameModeBase::SpawnEnemies, waveDelay);
}
void AActionChaosGameModeBase::SpawnEnemies()
{
	FActorSpawnParameters params;
	if (enemySpawnList.Num() == 0)
		return;
	int spawnType = enemySpawnList[enemySpawnIndex];
	AActor* enemy = GetWorld()->SpawnActor<AActor>(enemyTypes[spawnType], enemySpawnpoint, FRotator(0, FMath::RandRange(0, 999), 0), params);
	if (enemy)
	{
		AddEnemy(enemy, enemyValues[spawnType]);
		UE_LOG(Game, Warning, TEXT("spawned enemy of cost %d"), enemyCosts[spawnType]);
		enemySpawnIndex++;
		if (enemySpawnIndex < enemySpawnList.Num())
		{
			FTimerHandle nextSpawnTimer;
			GetWorld()->GetTimerManager().SetTimer(nextSpawnTimer, this, &AActionChaosGameModeBase::SpawnEnemies, spawnRate);
		}
	}
	else
	{
		UE_LOG(Game, Warning, TEXT("failed to spawn enemy of cost %d"), enemyCosts[spawnType]);
		FTimerHandle nextSpawnTimer;
		GetWorld()->GetTimerManager().SetTimer(nextSpawnTimer, this, &AActionChaosGameModeBase::SpawnEnemies, spawnRate * 2);
	}
}
///////////////////////////////////////////////////
/// game management functions
 