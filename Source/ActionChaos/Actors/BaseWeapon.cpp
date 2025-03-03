// Fill out your copyright notice in the Description page of Project Settings.


#include "../Actors/BaseWeapon.h"
#include "../Actors/BaseCharacter.h"
#include "../ActionChaos.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	AllWeapons = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	AllWeapons->SetupAttachment(GetRootComponent(), MuzzleSocket);
	Alive = true;
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	AActor* Weapon = this->GetParentActor();
	APawn* Pawn = Cast<APawn>(Weapon);
	if (Pawn)
	{
		ParentPawn = Pawn;
		currentAmmo = AmmoinMag;

		// Saves ammo data
		savedCurrAmmo = currentAmmo;
		savedMaxAmmo = MaxAmmo;
	}
	else
	{
		UE_LOG(Game, Error, TEXT("The owning Actor is not a valid pawn"));
		Destroy();
	}
}

void ABaseWeapon::UseAmmo()
{
	//currentAmmo = FMath::Max((currentAmmo - 1), 0);
	currentAmmo = FMath::Clamp((currentAmmo - 1), 0, AmmoinMag);
	OnAmmoChanged.Broadcast(currentAmmo, MaxAmmo);
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ABaseWeapon::IsAutomatic() const
{
	return bIsAutomatic;
}

bool ABaseWeapon::IsSemiAutomatic() const
{
	return bIsSemiAutomatic;
}

bool ABaseWeapon::IsShotgun() const
{
	return bIsShotgun;
}

float ABaseWeapon::GetTimer() const
{
	return ResetTimer;
}

FName ABaseWeapon::GetSocket() const
{
	if (WeaponSocket.IsNone())
	{
		UE_LOG(Game, Error, TEXT("WeaponSocket is not set"));
	}
	return WeaponSocket;
}

void ABaseWeapon::Shoot()
{
	FActorSpawnParameters Params;
	ABaseCharacter* Character = Cast<ABaseCharacter>(ParentPawn);
	if (Character)
	{
		Params.Owner = Character->GetController();
		Params.Instigator = Character;
	}
	if (currentAmmo != 0)
	{
		// Spread
		float RandomPitch = FMath::FRandRange(-spread, spread);
		float RandomYaw = FMath::FRandRange(-spread, spread);

		// Rotation
		FRotator SpreadRotation = ParentPawn->GetBaseAimRotation().Add(RandomPitch, RandomYaw, 0);

		if (ProjectileClass && bIsAutomatic) // Automatic
		{
			if (bIsShotgun) // Shotgun check
			{
				for (int i = 0; i < numberOfPellets; i++)
				{
					// Spread
					float ShotgunPitch = FMath::FRandRange(-spread, spread);
					float ShotgunYaw = FMath::FRandRange(-spread, spread);

					// Rotation
					FRotator ShotgunRotation = ParentPawn->GetBaseAimRotation().Add(ShotgunPitch, ShotgunYaw, 0);

					// Spawning projectile
					GetWorld()->SpawnActor<AActor>(ProjectileClass, AllWeapons->GetSocketLocation(MuzzleSocket), ShotgunRotation, Params);
				}
			}
			else
			{
				// Spawning projectile
				GetWorld()->SpawnActor<AActor>(ProjectileClass, AllWeapons->GetSocketLocation(MuzzleSocket), SpreadRotation, Params);
			}

			bIsFiring = true;
			OnWeaponAttack.Broadcast();
			UseAmmo();

			if (Character->IsAI())
			{
				GetWorld()->GetTimerManager().SetTimer(Reset, this, &ABaseWeapon::SemiStopShooting, ResetTimer, true);
			}
			else
			{
				GetWorld()->GetTimerManager().SetTimer(Reset, this, &ABaseWeapon::Shoot, ResetTimer, true);
			}
		}
		else if (ProjectileClass && bIsSemiAutomatic) // SemiAutomatic
		{
			if (bIsShotgun) // Shotgun check
			{
				for (int i = 0; i < numberOfPellets; i++)
				{
					// Spread
					float ShotgunPitch = FMath::FRandRange(-spread, spread);
					float ShotgunYaw = FMath::FRandRange(-spread, spread);

					// Rotation
					FRotator ShotgunRotation = ParentPawn->GetBaseAimRotation().Add(ShotgunPitch, ShotgunYaw, 0);

					// Spawning projectile
					GetWorld()->SpawnActor<AActor>(ProjectileClass, AllWeapons->GetSocketLocation(MuzzleSocket), ShotgunRotation, Params);
				}
			}
			else
			{
				// Spawning projectile
				GetWorld()->SpawnActor<AActor>(ProjectileClass, AllWeapons->GetSocketLocation(MuzzleSocket), SpreadRotation, Params);
			}
			bIsFiring = true;
			OnWeaponAttack.Broadcast();
			UseAmmo();
			GetWorld()->GetTimerManager().SetTimer(Reset, this, &ABaseWeapon::SemiStopShooting, ResetTimer);
		}
		else if (ProjectileClass && bIsBurst)
		{
			GetWorld()->SpawnActor<AActor>(ProjectileClass, AllWeapons->GetSocketLocation(MuzzleSocket), SpreadRotation, Params);
			currentShot++;
			bIsFiring = true;
			OnWeaponAttack.Broadcast();
			UseAmmo();
			if (currentShot < BulletsPerBurst)
			{
				GetWorld()->GetTimerManager().SetTimer(Reset, this, &ABaseWeapon::Shoot, 0.1f, true);
			}
			else
			{
				GetWorld()->GetTimerManager().SetTimer(Reset, this, &ABaseWeapon::SemiStopShooting, ResetTimer);
				currentShot = 0;
			}
		}
		OnWeaponAttack.Broadcast();
	}


	// No FireMode
	if (!bIsAutomatic && !bIsSemiAutomatic && !bIsBurst)
	{
		UE_LOG(Game, Error, TEXT("No fire mode selected"));
	}
	UE_LOG(Game, Warning, TEXT("I am Firing"));
}

void ABaseWeapon::StopShooting()
{
	if (bIsAutomatic)
	{
		bIsFiring = false;
		GetWorld()->GetTimerManager().ClearTimer(Reset);
	}
	OnActionStopped.Broadcast();
}

FVector ABaseWeapon::GetSource()
{
	return AllWeapons->GetSocketLocation("MuzzleFlashSocket");
}

int ABaseWeapon::GetCurrentAmmo()
{
	return currentAmmo;
}

int ABaseWeapon::GetMaxAmmo()
{
	return MaxAmmo;
}

void ABaseWeapon::OwnerDed()
{
	Alive = false;
}

bool ABaseWeapon::CanShoot() const
{
	if (!bIsFiring && Alive)
	{
		return true;
	}
	return false;
}

void ABaseWeapon::Reload()
{
	//if (!Action)
	//{
	//	Action = true;
	//	OnReload.Broadcast();
	//}
	//if (currentAmmo < MaxAmmo)
	//{
	//}
	OnReload.Broadcast();

	// Ammo consumption logic
	if (MaxAmmo <= 0)
	{
		return;
	}

	int fN2 = AmmoinMag - currentAmmo; // How much ammo need to refill
	int fN = FMath::Min(fN2, MaxAmmo); // Math

	// Answer
	currentAmmo += fN;
	MaxAmmo -= fN;

	OnAmmoChanged.Broadcast(currentAmmo, MaxAmmo);
}

void ABaseWeapon::AIReload() // AI has unlimited ammo
{
	//if (!Action)
	//{
	//	Action = true;
	//	OnReload.Broadcast();
	//}
	//if (currentAmmo < MaxAmmo)
	//{
	//}

	OnReload.Broadcast();
	currentAmmo = savedCurrAmmo;
	OnAmmoChanged.Broadcast(currentAmmo, MaxAmmo);
}

void ABaseWeapon::ReloadPercent(float percent)
{
	MaxAmmo = FMath::Clamp(savedCurrAmmo + savedMaxAmmo * percent, 0., savedMaxAmmo);
	OnAmmoChanged.Broadcast(currentAmmo, MaxAmmo);
}

void ABaseWeapon::SemiStopShooting()
{
	bIsFiring = false;
	OnActionStopped.Broadcast();
}