// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "HWeaponBase.h"
#include "TimerManager.h"
#include "Prototype.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "Engine.h"



// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* Setting up the components */
	CamSpringComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring"));
	CamSpringComp->SetupAttachment(RootComponent);
	CamSpringComp->TargetArmLength = 0.0f;
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(CamSpringComp);
	CameraComp->bUsePawnControlRotation = true;
	MeshLagComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("MeshLagComp"));
	MeshLagComp->SetupAttachment(CameraComp);
	MeshLagComp->bEnableCameraLag = true;
	MeshLagComp->bEnableCameraRotationLag = true;
	MeshLagComp->TargetArmLength = 0.0f;
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1P"));
	Mesh1P->SetupAttachment(MeshLagComp);
	MouseSensitivity = 1.0;
	bIsReloading = false;
	DropForceComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Drop Comp"));
	DropForceComp->SetupAttachment(RootComponent);

	DropForce = DropForceComp->GetForwardVector();

	bIsTargeting = false;
	DefaultMaxSpeed = GetCharacterMovement()->MaxWalkSpeed;
	bCanRun = true;
	bIsRunning = false;

	TargetingMouseSensitivity = 0.5;

	MinWalkingSpread = 5.f;

	MinFallingSpread = 10.f;

	DefaultFOV = CameraComp->FieldOfView;

}



// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon(StartingWeaponClass);
}


// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ABaseCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &ABaseCharacter::LookRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABaseCharacter::OnJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ABaseCharacter::OnStopJump);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ABaseCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ABaseCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABaseCharacter::StartWeaponFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ABaseCharacter::StopWeaponFire);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ABaseCharacter::OnReload);
	PlayerInputComponent->BindAction("Drop", IE_Pressed, this, &ABaseCharacter::DropWeapon);

	PlayerInputComponent->BindAction("Targeting", IE_Pressed, this, &ABaseCharacter::OnBeginTargeting);
	PlayerInputComponent->BindAction("Targeting", IE_Released, this, &ABaseCharacter::OnEndTargeting);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ABaseCharacter::StartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ABaseCharacter::StopRunning);
}


void ABaseCharacter::SpawnWeapon(TSubclassOf<class AHWeaponBase> WeaponClass)
{

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AHWeaponBase* SpawnedWeapon;
	SpawnedWeapon = Cast<AHWeaponBase>(GetWorld()->SpawnActor<AActor>(WeaponClass, GetActorTransform(), SpawnParams));
	SpawnedWeapon->SetOwningPawn(this);
	AddToInventory(SpawnedWeapon, GetFirstEmptySlot());
	if (SpawnedWeapon)
	{
		AttachWeapon(SpawnedWeapon);

	}


}


void ABaseCharacter::StartWeaponFire()
{
	//UE_LOG(LogTemp, Warning, TEXT("FIRING WEAPON"));
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFire();
		
	}
}


void ABaseCharacter::StopWeaponFire()
{
	//UE_LOG(LogTemp, Warning, TEXT("Stoped Firing the Waepon !"));
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
		bIsFiring = false;
	}
}

void ABaseCharacter::OnReload()
{
	if (CurrentWeapon && !bIsReloading)
	{
		bIsReloading = true;
		CurrentWeapon->Reload();
	}
}




void ABaseCharacter::AttachWeapon(AHWeaponBase* Weapon)
{
	if (Weapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("attaching weapon !"));
		FName SocketName = Weapon->GetAttachementSocketName();
		Weapon->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
	}

}


FVector ABaseCharacter::GetPawnViewLocation() const
{
	if (CameraComp)
	{
		return CameraComp->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}

bool ABaseCharacter::AddToInventory(AHWeaponBase* Item, int32 Index)
{
	if (Item)
	{
		Inventory.AddUnique(Item);
	}


	return true;
}

int32 ABaseCharacter::GetFirstEmptySlot()
{
	int32 Index = 0;
	for (int32 I = 0; I < Inventory.Num(); ++I)
	{
		I = 0;
		AHWeaponBase* Ref = Inventory[I];
		if (Ref != nullptr)
		{
			I++;
		}
		else
		{
			Index = I;
			break;
		}

	}

	return Index;
}

AHWeaponBase* ABaseCharacter::GetFromInventory(int32 Index)
{
	return Inventory[Index];
}

bool ABaseCharacter::RemoveFromInventory(AHWeaponBase* Weapon)
{
	Inventory.Remove(Weapon);
		return true;
}

void ABaseCharacter::EquipWeapon(AHWeaponBase * Weapon)
{
	if (Weapon)
	{
		if (!CurrentWeapon)
		{
			CurrentWeapon = Weapon;
			CurrentWeapon->OnEquip();
			AttachWeapon(CurrentWeapon);
		}
		if (CurrentWeapon && CurrentWeapon != Weapon )
		{
			CurrentWeapon->OnUnequip();
			AttachWeapon(CurrentWeapon);
			CurrentWeapon = Weapon;
			CurrentWeapon->OnEquip();
			AttachWeapon(CurrentWeapon);
		}

	}
}

void ABaseCharacter::EquipNext()
{
	APlayerController* MyPC = Cast<APlayerController>(Controller);
	if (MyPC)
	{
		if (Inventory.Num() >= 2)
		{
			int32 Index = (CurrentInventoryIndex + 1) % Inventory.Num();
			if (Inventory.IsValidIndex(Index))
			{
				AHWeaponBase* NextWeapon = Inventory[Index];
				if (NextWeapon)
				{
					EquipWeapon(NextWeapon);
					CurrentInventoryIndex = Index;
					AttachWeapon(NextWeapon);
				}

			}
			else
			{
				UE_LOG(LogWeapon, Warning, TEXT("Invalid index for next weapon"))
			}


		}
		else
		{
			AHWeaponBase* Weap = Inventory[0];
			if (Weap)
			{
				EquipWeapon(Weap);
				AttachWeapon(Weap);
			}

		}
	}


}

void ABaseCharacter::EquipLast()
{

	APlayerController* MyPC = Cast<APlayerController>(Controller);
	if (MyPC && Inventory.Num() >= 2)
	{
		int32 Index = (CurrentInventoryIndex - 1 + Inventory.Num()) % Inventory.Num();
		if (Inventory.IsValidIndex(Index))
		{
			AHWeaponBase* NextWeapon = Inventory[Index];
			if (NextWeapon)
			{
				EquipWeapon(NextWeapon);
				CurrentInventoryIndex = Index;
			}

		}
		else
		{
			UE_LOG(LogWeapon, Warning, TEXT("Invalid index for Last weapon"))
		}


	}

}



//Add Movement to the character

void ABaseCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}


void ABaseCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}


void ABaseCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value * MouseSensitivity);
}


void ABaseCharacter::LookRight(float Value)
{
	AddControllerYawInput(Value * MouseSensitivity);
}


void ABaseCharacter::OnJump_Implementation()
{
	if (CanJump() & !bIsJumping)
	{
		Jump();
		bIsJumping = true;
	}
}


void ABaseCharacter::OnStopJump_Implementation()
{
	StopJumping();
	bIsJumping = false;
}


void ABaseCharacter::BeginCrouch()
{
	Crouch();
}


void ABaseCharacter::EndCrouch()
{
	UnCrouch();
}


bool ABaseCharacter::CanUncrouch()
{
	//check if we can un crouch !
	FHitResult HitRes;
	FVector StartLoc = GetActorLocation();
	FVector EndLoc = (GetActorUpVector()*88.0f) + StartLoc;

	FCollisionObjectQueryParams ObjQuery;
	ObjQuery.AllObjects;
	//GetWorld()->
	//DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Red, false, 5.0, (uint8)'\000', 3.0);
	return !(HitRes.bBlockingHit);
}


void ABaseCharacter::SetMouseSensitivity(float newSensitivity)
{
	MouseSensitivity = FMath::Clamp(newSensitivity, 0.f, 10.f);
}

float ABaseCharacter::PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	
	if (AnimMontage && Mesh1P && Mesh1P->AnimScriptInstance)
	{
		return Mesh1P->AnimScriptInstance->Montage_Play(AnimMontage, InPlayRate);
	}

	return 0.0f;
}

void ABaseCharacter::StopAnimMontage(class UAnimMontage* AnimMontage)
{
	
	if (AnimMontage && Mesh1P && Mesh1P->AnimScriptInstance &&
		Mesh1P->AnimScriptInstance->Montage_IsPlaying(AnimMontage))
	{
		Mesh1P->AnimScriptInstance->Montage_Stop(AnimMontage->BlendOut.GetBlendTime(), AnimMontage);
	}
}

void ABaseCharacter::OnDropWeapon(AHWeaponBase * Weapon)
{
	if (Weapon)
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		UStaticMeshComponent* SM = Cast<UStaticMeshComponent>(Weapon->GetRootComponent());
		if (SM)
		{
			SM->AddImpulse(DropForceStrenght*DropForce);
		}
		Weapon->SetOwner(nullptr);
	}
}

void ABaseCharacter::DropWeapon()
{
	if (CurrentWeapon && CurrentWeapon->bCanBeDroped)
	{
		UStaticMeshComponent* WeaponMesh = Cast<UStaticMeshComponent>(CurrentWeapon->GetMesh());
		if (WeaponMesh)
		{
			WeaponMesh->SetSimulatePhysics(true);
		}
		
		OnDropWeapon(CurrentWeapon);
		RemoveFromInventory(CurrentWeapon);
		EquipNext();
	}
}

void ABaseCharacter::SetTargeting(bool NewTargeting)
{
	bIsTargeting = NewTargeting;
}

bool ABaseCharacter::GetTargeting()
{
	return bIsTargeting;
}

void ABaseCharacter::OnBeginTargeting()
{
	SetTargeting(true);
	SetMaxSpeed(TargetingSpeedModif);
	float FOV = 70.f;
	if (CurrentWeapon)
	{
		FOV = CurrentWeapon->TargetingFOV;
		CurrentWeapon->OnStartTargeting();
	}
	CameraComp->SetFieldOfView(FOV);
}

void ABaseCharacter::OnEndTargeting()
{
	SetTargeting(false);
	SetMaxSpeed(1.0);
	if (CurrentWeapon)
	{
		CurrentWeapon->OnStopTargeting();
	}
	CameraComp->SetFieldOfView(DefaultFOV);
}

void ABaseCharacter::SetMaxSpeed(float SpeedModifier)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = DefaultMaxSpeed * SpeedModifier;
	}
}

void ABaseCharacter::StartRunning()
{
	if (bCanRun)
	{
		if (bIsTargeting)
		{
			OnEndTargeting();
		}
		SetMaxSpeed(RunningSpeedModif);
		if (CurrentWeapon)
		{
			CurrentWeapon->bCanFire = false;
		}
		bIsRunning = true;
	}
}

void ABaseCharacter::StopRunning()
{
	SetMaxSpeed(1);
	if (CurrentWeapon)
	{
		CurrentWeapon->bCanFire = true;
	}
	bIsRunning = false;
}
