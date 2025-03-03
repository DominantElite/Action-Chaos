// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "../Utility/CharacterAnimation.h"
#include "../Widgets/PlayerHUDBase.h"
#include "../ActionChaos.h"
#include <ActionChaos/ActionChaosGameModeBase.h>
#include <Kismet/GameplayStatics.h>
#include <ActionChaos/Utility/InteractInterface.h>

ABasePlayer::ABasePlayer()
{
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(GetMesh(), "CameraSocket");
}

void ABasePlayer::RemoveSprint() // Function will be used in the future
{
	if (InputComponent)
	{
		InputComponent->RemoveActionBinding("Run", IE_Pressed);
		InputComponent->RemoveActionBinding("Run", IE_Released);
	}
}

void ABasePlayer::BeginPlay()
{
	Super::BeginPlay();
	playerController = Cast<APlayerController>(GetController());
	if (playerController)
	{
		if (WidgetClass)
		{
			WidgetObject = CreateWidget(playerController, WidgetClass);
			if (WidgetObject)
			{
				WidgetObject->AddToViewport();
				Weapon->OnAmmoChanged.AddDynamic(Cast<UPlayerHUDBase>(WidgetObject), &UPlayerHUDBase::SetAmmo);

				Weapon->OnAmmoChanged.Broadcast(Weapon->GetCurrentAmmo(), Weapon->GetMaxAmmo());

				if (!HealthComp)
				{
					UE_LOG(Game, Error, TEXT("Health is not real"));
					Destroy();
				}
				HealthComp->OnHeal.AddDynamic(Cast<UPlayerHUDBase>(WidgetObject), &UPlayerHUDBase::SetHealth);
				HealthComp->OnDeath.AddDynamic(Cast<UPlayerHUDBase>(WidgetObject), &UPlayerHUDBase::SetHealth);
				if (ArmorBody && (ArmorBody->Armor == EArmorType::Spartan))
				{
					//ArmorBody->OnShieldshit.AddDynamic(Cast<UPlayerHUDBase>(WidgetObject), &UPlayerHUDBase::SetShield);
					ArmorBody->OnShieldsDown.AddDynamic(Cast<UPlayerHUDBase>(WidgetObject), &UPlayerHUDBase::SetShield);
				}
				AActionChaosGameModeBase* gamemode = Cast<AActionChaosGameModeBase>(GetWorld()->GetAuthGameMode());
				if (gamemode) {
					gamemode->OnWaveChanged.AddDynamic(Cast<UPlayerHUDBase>(WidgetObject), &UPlayerHUDBase::SetWaves);
					Cast<UPlayerHUDBase>(WidgetObject)->SetWaves(gamemode->currentWave, gamemode->maxWaves);
					gamemode->OnEnemySpawned.AddDynamic(Cast<UPlayerHUDBase>(WidgetObject), &UPlayerHUDBase::SetEnemyCount);
					Cast<UPlayerHUDBase>(WidgetObject)->SetEnemyCount(0, gamemode->GetCurrentWave().Num());
				}
				if (WeaponClass == StarterPistolClass)
					Cast<UPlayerHUDBase>(WidgetObject)->SetWeaponImage(EShopItem::StartPistol);
				else if (WeaponClass == RifleClass)
					Cast<UPlayerHUDBase>(WidgetObject)->SetWeaponImage(EShopItem::Rifle);
				else if (WeaponClass == ShotgunClass)
					Cast<UPlayerHUDBase>(WidgetObject)->SetWeaponImage(EShopItem::Shotgun);
				else if (WeaponClass == AutoPistolClass)
					Cast<UPlayerHUDBase>(WidgetObject)->SetWeaponImage(EShopItem::AutoPistol);
				else if (WeaponClass == AutoShotgunClass)
					Cast<UPlayerHUDBase>(WidgetObject)->SetWeaponImage(EShopItem::AutoShotgun);
				else if (WeaponClass == SemiRifleClass)
					Cast<UPlayerHUDBase>(WidgetObject)->SetWeaponImage(EShopItem::SemiRifle); 
				else if (WeaponClass == BurstRifleClass)
					Cast<UPlayerHUDBase>(WidgetObject)->SetWeaponImage(EShopItem::BurstRifle);
				CharacterAnimation->OnDeathEnded.AddDynamic(this, &ABasePlayer::PlayerLost);
			}
			else
			{
				UE_LOG(Game, Error, TEXT("We cannot show player HUD"));
				Destroy();
			}
		}
		else
		{
			UE_LOG(Game, Error, TEXT("We cannot create player HUD"));
			Destroy();
		}
	}
	else
	{
		UE_LOG(Game, Warning, TEXT("Not a PlayerController"));
		Destroy();
	}

	// Temp test
	if (ArmorLegsClass && (ArmorLegs->Armor == EArmorType::Spartan))
	{
		RemoveSprint();
		CharacterAnimation->bIsRunning = true;
	}

	FName AttachedSocketName = ChildActorComponent->GetAttachSocketName();
	UE_LOG(LogTemp, Warning, TEXT("Player's weapon is attached to socket: %s"), *AttachedSocketName.ToString());
}

void ABasePlayer::HandleHurt(float health)
{
	Super::HandleHurt(health);
	Cast<UPlayerHUDBase>(WidgetObject)->SetHealth(health);
}

void ABasePlayer::HandleShields(float shield)
{
	Super::HandleShields(shield);
	Cast<UPlayerHUDBase>(WidgetObject)->SetShield(shield);
}

void ABasePlayer::HandleDeathStart(float health)
{
	Super::HandleDeathStart(health);
	playerController->DisableInput(playerController);
}

void ABasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// WASD
	PlayerInputComponent->BindAxis("TurnRight", this, &ABasePlayer::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ABasePlayer::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("MoveForward", this, &ABasePlayer::InputAxisMoveForward);
	PlayerInputComponent->BindAxis("Strafe", this, &ABasePlayer::InputAxisStrafe);

	// Shooting
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABasePlayer::Fire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ABasePlayer::InputActionStopFiring);

	// Interacting
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ABasePlayer::InputActionInteractPressed);

	// Reloading
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ABasePlayer::InputActionReloadPressed);

	// Jumping
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABasePlayer::InputActionJumpPressed);

	// Crouching
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ABasePlayer::InputActionCrouch);

	// Running
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ABasePlayer::InputActionRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ABasePlayer::InputActionRunning);
}

void ABasePlayer::PlayerWin()
{
	DisableInput(playerController);
	WidgetObject->RemoveFromParent();
}

//FRotator ABasePlayer::GetBaseAimRotation() const
//{
//	if (WidgetObject)
//		return FRotationMatrix::MakeFromX(Cast<UPlayerHUDBase>(WidgetObject)->GetDestination() - Weapon->GetSource()).Rotator();
//	else
//		return Super::GetBaseAimRotation();
//}

void ABasePlayer::EquipWeapon(TEnumAsByte<EShopItem::Item> weapon)
{
	Weapon->OnAmmoChanged.RemoveDynamic(Cast<UPlayerHUDBase>(WidgetObject), &UPlayerHUDBase::SetAmmo);
	Cast<UPlayerHUDBase>(WidgetObject)->SetWeaponImage(weapon);
	switch (weapon) {
	case EShopItem::StartPistol:
		WeaponClass = StarterPistolClass;
		break;
	case EShopItem::Rifle:
		WeaponClass = RifleClass;
		break;
	case EShopItem::Shotgun:
		WeaponClass = ShotgunClass;
		break;
	case EShopItem::AutoPistol:
		WeaponClass = AutoPistolClass;
		break;
	case EShopItem::AutoShotgun:
		WeaponClass = AutoShotgunClass;
		break;
	case EShopItem::SemiRifle:
		WeaponClass = SemiRifleClass;
		break;
	case EShopItem::BurstRifle:
		WeaponClass = BurstRifleClass;
		break;
	default:
		UE_LOG(Game, Error, TEXT("Weapon equip ID was unexpected"))
	}
	ChildActorComponent->SetChildActorClass(WeaponClass);
	Weapon = Cast<ABaseWeapon>(ChildActorComponent->GetChildActor());
	if (Weapon)
	{
		weaponSocketName = Weapon->GetSocket();
		ChildActorComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, weaponSocketName);
	}
	Weapon->OnAmmoChanged.AddDynamic(Cast<UPlayerHUDBase>(WidgetObject), &UPlayerHUDBase::SetAmmo);
	Weapon->Reload();
}

void ABasePlayer::EquipArmor(TEnumAsByte<EShopItem::Item> armor)
{
	switch (armor) {
	case EShopItem::SPHead:
		ArmorHeadClass = SpartanHelmetClass;
		HeadChildActorComponent->SetChildActorClass(SpartanHelmetClass);
		ArmorHead = Cast<ABaseArmor>(HeadChildActorComponent->GetChildActor());
		if (ArmorHead)
		{
			HeadSocketName = ArmorHead->GetSocket();
			HeadChildActorComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, HeadSocketName);
		}
	case EShopItem::SPChest:
		BodyChildActorComponent->SetChildActorClass(ArmorBodyClass);
		ArmorBody = Cast<ABaseArmor>(BodyChildActorComponent->GetChildActor());
		if (ArmorBody)
		{
			BodySocketName = ArmorBody->GetSocket();
			BodyChildActorComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, BodySocketName);
			HealthComp->GetBodyArmor();
		}
	case EShopItem::SPLeg:
		LegsChildActorComponent->SetChildActorClass(ArmorLegsClass);
		ArmorLegs = Cast<ABaseArmor>(LegsChildActorComponent->GetChildActor());
		if (ArmorLegs)
		{
			LegsSocketName = ArmorLegs->GetSocket();
			LegsChildActorComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, LegsSocketName);
		}
	}
}

void ABasePlayer::InputAxisMoveForward(float AxisValue)
{
	if (CharacterAnimation->bIsRunning)
	{
		AddMovementInput(FRotator(0., GetControlRotation().Yaw, 0.).Vector(), AxisValue * 2);
	}
	else if (CharacterAnimation->bIsCrouching)
	{
		AddMovementInput(FRotator(0., GetControlRotation().Yaw, 0.).Vector(), AxisValue);
	}
	else
	{
		AddMovementInput(FRotator(0., GetControlRotation().Yaw, 0.).Vector(), AxisValue);
	}
}
void ABasePlayer::InputAxisStrafe(float AxisValue)
{
	if (CharacterAnimation->bIsRunning)
	{
		AddMovementInput(FRotationMatrix(FRotator(0., GetControlRotation().Yaw, 0.)).GetScaledAxis(EAxis::Y), AxisValue * 2);
	}
	else if (CharacterAnimation->bIsCrouching)
	{
		AddMovementInput(FRotationMatrix(FRotator(0., GetControlRotation().Yaw, 0.)).GetScaledAxis(EAxis::Y), AxisValue);
	}
	else
	{
		AddMovementInput(FRotationMatrix(FRotator(0., GetControlRotation().Yaw, 0.)).GetScaledAxis(EAxis::Y), AxisValue);
	}
}

void ABasePlayer::Fire()
{
	if (Weapon && Weapon->CanShoot())
	{
		Weapon->Shoot();
	}
}

void ABasePlayer::InputActionStopFiring()
{
	Weapon->StopShooting();
}

void ABasePlayer::InputActionCrouch()
{
	CharacterAnimation->bIsCrouching = !CharacterAnimation->bIsCrouching;
}

void ABasePlayer::InputActionRunning()
{
	CharacterAnimation->bIsRunning = !CharacterAnimation->bIsRunning;
}

void ABasePlayer::InputActionReloadPressed()
{
	Weapon->Reload();
}

void ABasePlayer::InputActionInteractPressed()
{
	IInteractInterface* interactable = Cast<IInteractInterface>(Cast<UPlayerHUDBase>(WidgetObject)->GetTarget());
	if (interactable)
		interactable->Interact();
}

void ABasePlayer::InputActionJumpPressed()
{
	Jump();
	if (ArmorLegsClass && ArmorLegs->Armor == EArmorType::Spartan)
	{
		UCharacterMovementComponent* jumpcom = this->GetCharacterMovement();
		if (jumpcom)
		{
			jumpcom->JumpZVelocity = 800.f;
		}
	}
}

void ABasePlayer::PlayerLost()
{
	playerController->bShowMouseCursor = true;
	WidgetObject->RemoveFromParent();
	OnPlayerLost.Broadcast();
}

void ABasePlayer::RefillMaxHealth(float percentage)
{
	UGameplayStatics::ApplyDamage(this, -HealthComp->GetHealAmount(percentage), GetController(), this, UDamageType::StaticClass());
}

void ABasePlayer::RefillMaxAmmo(float percentage)
{
	Weapon->ReloadPercent(percentage);
}
