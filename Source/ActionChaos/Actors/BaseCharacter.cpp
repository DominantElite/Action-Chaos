// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "../ActionChaos.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Utility/CharacterAnimation.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->SetRelativeRotation(FRotator(0., -90., 0.));
	GetMesh()->SetRelativeLocation(FVector(0., 0., -90.));

	// Weapon and its placement
	ChildActorComponent = CreateDefaultSubobject<UChildActorComponent>("ChildActorComponent");
	//ChildActorComponent->SetupAttachment(GetMesh(), socketName);

	// Armor
	HeadChildActorComponent = CreateDefaultSubobject<UChildActorComponent>("HeadChildActorComponent");
	BodyChildActorComponent = CreateDefaultSubobject<UChildActorComponent>("BodyChildActorComponent");
	LegsChildActorComponent = CreateDefaultSubobject<UChildActorComponent>("LegsChildActorComponent");

	HeadChildActorComponent->ComponentTags.Add(FName("Head"));
	BodyChildActorComponent->ComponentTags.Add(FName("Body"));
	LegsChildActorComponent->ComponentTags.Add(FName("Legs"));

	HealthComp = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Weapon
	if (WeaponClass)
	{
		ChildActorComponent->SetChildActorClass(WeaponClass);
		Weapon = Cast<ABaseWeapon>(ChildActorComponent->GetChildActor());
		if (Weapon)
		{
			weaponSocketName = Weapon->GetSocket();
			ChildActorComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, weaponSocketName);
		}
		else
		{
			UE_LOG(Game, Error, TEXT("The Weapon can't be found in BaseCharacter :("));
			Weapon->Destroy();
		}
	}
	else
	{
		UE_LOG(Game, Warning, TEXT("No Weapon class in BaseCharacter.cpp"));
		Destroy();
	}

	// Head Armor
	if (ArmorHeadClass)
	{
		HeadChildActorComponent->SetChildActorClass(ArmorHeadClass);
		ArmorHead = Cast<ABaseArmor>(HeadChildActorComponent->GetChildActor());
		if (ArmorHead)
		{
			HeadSocketName = ArmorHead->GetSocket();
			HeadChildActorComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, HeadSocketName);
		}
		else
		{
			UE_LOG(Game, Error, TEXT("The Armor can't be found in BaseCharacter :("));
			ArmorHead->Destroy();
		}
	}

	// Body Armor
	if (ArmorBodyClass)
	{
		BodyChildActorComponent->SetChildActorClass(ArmorBodyClass);
		ArmorBody = Cast<ABaseArmor>(BodyChildActorComponent->GetChildActor());
		if (ArmorBody)
		{
			BodySocketName = ArmorBody->GetSocket();
			BodyChildActorComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, BodySocketName);
			HealthComp->GetBodyArmor();
		}
		else
		{
			UE_LOG(Game, Error, TEXT("The Armor can't be found in BaseCharacter :("));
			ArmorBody->Destroy();
		}
	}

	if (ArmorLegsClass)
	{
		LegsChildActorComponent->SetChildActorClass(ArmorLegsClass);
		ArmorLegs = Cast<ABaseArmor>(LegsChildActorComponent->GetChildActor());
		if (ArmorLegs)
		{
			LegsSocketName = ArmorLegs->GetSocket();
			LegsChildActorComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, LegsSocketName);
		}
		else
		{
			UE_LOG(Game, Error, TEXT("The Armor can't be found in BaseCharacter :("));
			ArmorLegs->Destroy();
		}
	}

	CharacterAnimation = Cast<UCharacterAnimation>(GetMesh()->GetAnimInstance());
	if (CharacterAnimation)
	{
		Weapon->OnWeaponAttack.AddDynamic(this, &ABaseCharacter::HandleRifleAttack);
		HealthComp->OnHurt.AddDynamic(this, &ABaseCharacter::HandleHurt);
		if (ArmorBody && ArmorBody->Armor == EArmorType::Spartan)
		{
			ArmorBody->OnShieldshit.AddDynamic(this, &ABaseCharacter::HandleShields);
		}
		HealthComp->OnDeath.AddDynamic(this, &ABaseCharacter::HandleDeathStart);
		Weapon->OnReload.AddDynamic(CharacterAnimation, &UCharacterAnimation::ReloadAnimation);
	}
	else
	{
		UE_LOG(Game, Warning, TEXT("CharacterAnimation is not a UCharacterAnimation"));
		Destroy();
	}

	if (ChildActorComponent->GetAttachSocketName() != weaponSocketName)
	{
		UE_LOG(Game, Warning, TEXT("ChildActorComponent is not attached to the expected socket: %s"), *weaponSocketName.ToString());
		UE_LOG(Game, Warning, TEXT("ChildActorComponent is attached to %s"), *ChildActorComponent->GetAttachSocketName().ToString());
	}
}

void ABaseCharacter::enableArmorPices()
{
	//Armor->Head(bEnableHemlet);
	//Armor->Body(bEnableBody);
	//Armor->Leg(bEnableLeg);
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Recharge time for shields // Temporary
	//if (ArmorBodyClass && ArmorBody->Armor == EArmorType::Spartan)
	//{
	//	float temp = ArmorBody->GetCurrentShield();
	//	if (temp < ArmorBody->GetMaxShield())
	//	{
	//		if (ArmorBody->canRechargeShields)
	//		{
	//			temp = FMath::FInterpConstantTo(temp, ArmorBody->GetMaxShield(), DeltaTime, ArmorBody->GetRechargeTime());
	//		}
	//	}
	//}
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool ABaseCharacter::IsAI()
{
	return bisAI;
}

void ABaseCharacter::HandleRifleAttack()
{
	if (CharacterAnimation)
	{
		CharacterAnimation->FireAnimation();
		UE_LOG(Game, Warning, TEXT("I AM FireAnimation"));
	}
	else
	{
		UE_LOG(Game, Error, TEXT("No animations. BIG OOF"));
	}
}

void ABaseCharacter::HandleHurt(float health)
{
	if (CharacterAnimation)
	{
		CharacterAnimation->HitAnimation();
	}
}

// A Test to make sure it works
void ABaseCharacter::HandleShields(float shield)
{
	//if (CharacterAnimation)
	//{
	//	CharacterAnimation->HitAnimation();
	//}
}

void ABaseCharacter::HandleDeathStart(float health)
{
	Destroy(); // Temporary command

	// Real stuff
	//CharacterAnimation->DeadAnimation();
	//if (GetCharacterMovement())
	//{
	//	GetCharacterMovement()->DisableMovement();
	//}
	//if (GetController())
	//{
	//	GetController()->Destroy();
	//}
	////SetActorEnableCollision(false);
	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Weapon->OwnerDed();
}

