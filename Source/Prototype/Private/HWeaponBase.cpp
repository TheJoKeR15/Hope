// Fill out your copyright notice in the Description page of Project Settings.

#include "HWeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "BaseCharacter.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/HBaseImpact.h"
#include "Prototype.h"
#include "Animation/AnimMontage.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Weapons/HBaseProjectile.h"
#include "Kismet/KismetMathLibrary.h"




// Sets default values
AHWeaponBase::AHWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	RootComponent = SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	MeshComp->SetupAttachment(RootComponent);

	WeaponRange = 10000.f;
	MaxPenetration = 500.f;
	bCanBeDroped = true;

	SpreadDecreaseInterp = 10.f;

	TargetingFOV = 50.f;

}

// Called when the game starts or when spawned
void AHWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	bCanFire = true;
	bCanReload = true;
}

// Called every frame
void AHWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float MinSpread = 0;

	/** Set the min walking spread : 0 if we are standing still Speed <= 100.f
	 MinWalkingSprea if we are walking Speed >= 100.f
	 MinSpread Falling when we are falling (Jumping) or we are running */

	if (MyPawn->GetCharacterMovement()->IsFalling() || MyPawn->bIsRunning)
	{
		MinSpread = MyPawn->MinFallingSpread;
	}
	else if (MyPawn->GetVelocity().Size() > 10.f)
	{
		MinSpread = MyPawn->MinWalkingSpread;
	}
	float NewSpread = FMath::FInterpTo(SpreadV, MinSpread, DeltaTime, SpreadDecreaseInterp);

	SetSpread(NewSpread);

}

void AHWeaponBase::StartFire()
{

}

void AHWeaponBase::StopFire()
{
}

void AHWeaponBase::Fire()
{
	if (MyPawn)
	{
		if (CurrentAmmoinClip > 0 && CurrentAmmo >= 0)
		{
			// we get the aim informations
			FVector ViewLocation;
			FRotator ViewDirection;
			MyPawn->GetActorEyesViewPoint(ViewLocation, ViewDirection);
			FVector ShotDir = ViewDirection.Vector();
			//add Spread !
			ShotDir = FMath::VRandCone(ShotDir, SpreadV / 2.5, SpreadV);
			// calculate the crosshair location
			FVector EndLoc = ViewLocation + (ShotDir * WeaponRange);
			FVector ImpactPoint = EndLoc;
			// collision parameters
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(this);
			QueryParams.AddIgnoredActor(MyPawn);
			QueryParams.bTraceComplex = true;
			FHitResult Hit;
			FVector SpawnLoc;
			FRotator SpawnRot = ViewDirection;
			// trace against the world for objects that resond with OVERLAP to the weapon trace 
			if (GetWorld()->LineTraceSingleByChannel(Hit, ViewLocation, EndLoc, COLLISION_WEAPON, QueryParams))
			{
				ImpactPoint = Hit.ImpactPoint;
			}
			
			// we offset the location of the muzzel by 2 cm so we dont hit the weapon
			SpawnLoc = MeshComp->GetSocketLocation(MuzzleFlashSocketName) + (MeshComp->GetSocketRotation(MuzzleFlashSocketName).Vector()*2.f);
			SpawnRot = UKismetMathLibrary::FindLookAtRotation(SpawnLoc, ImpactPoint);
				/* Spawn The bullet ! */
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AHBaseProjectile* SpawnedBullet;
			SpawnedBullet = Cast<AHBaseProjectile>(GetWorld()->SpawnActor(BulletClass,&SpawnLoc,&SpawnRot,SpawnParams));
			if (SpawnedBullet)
			{
				SpawnedBullet->SetOwner(this);
				SpawnedBullet->Instigator = MyPawn;
				SpawnedBullet->SetOwnerWeapon(this);
				SpawnedBullet->SetOwnerPawn(MyPawn);
			}

			DrawDebugLine(GetWorld(), ViewLocation, ImpactPoint, FColor::Red, false, 3.0f);
			// play the local effcts (Muzzel flash)
			PlayEffects(ImpactPoint);
			CurrentAmmoinClip--;

			MyPawn->bIsFiring = true;
			// play the animation
			if (MyPawn && FireAnim && AimFireAnim)
			{
				PlayCharAnimation(MyPawn->bIsTargeting ? AimFireAnim : FireAnim);
			}

		}
		else if (CurrentAmmo > 0)
		{
			// reload if we dont have ammo
			Reload();

		}
		else if (EmptySound)
		{
			// play the empty sound
			UGameplayStatics::SpawnSoundAttached(EmptySound, RootComponent);
		}

	}

}

USceneComponent * AHWeaponBase::GetMesh()
{
	return MeshComp;
}

void AHWeaponBase::AddSpread(float Delta)
{
	SetSpread(SpreadV + Delta);
}

void AHWeaponBase::SetSpread(float NewSpread)
{

	SpreadV = FMath::Clamp(FMath::DegreesToRadians(NewSpread), 0.f, MaxSpread);
	if (MyPawn && MyPawn->bIsTargeting)
	{
		SpreadV *= FMath::Min(MyPawn->TargetingSpreadModif, MaxSpread);
	}
	if (MyPawn && MyPawn->bIsCrouched)
	{
		SpreadV *= FMath::Min(MyPawn->CrouchingSpreadModif, MaxSpread);
	}
}

void AHWeaponBase::OnStartTargeting()
{
	//increase acuracy !
}

void AHWeaponBase::OnStopTargeting()
{
	// return to normal
}

void AHWeaponBase::PlayEffects(FVector ImpactPoint)
{
	// spawn fire sound
	if (FireSound)
	{
		UGameplayStatics::SpawnSoundAttached(FireSound, RootComponent);
	}
	// spawn the muzzel flash particle sys
	if (MuzzleFlashEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlashEffect, MeshComp, MuzzleFlashSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTargetIncludingScale);
	}
	// spawn the Trail effect !
	if (TrailEffect)
	{
		UParticleSystemComponent* TracerEffect = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TrailEffect, GetMesh()->GetSocketTransform(MuzzleFlashSocketName));
		if (TracerEffect)
		{
			TracerEffect->SetVectorParameter("Target", ImpactPoint);
		}
	}
	// we apply a camera Shake !!
	APlayerController* PC = Cast<APlayerController>(MyPawn->GetController());
	if (PC)
	{
		PC->ClientPlayCameraShake(FireCamShake, FireShakeIntensity);
	}

}


void AHWeaponBase::Reload()
{

	if (CurrentAmmo > 0 && CurrentAmmoinClip != MaxAmmoPerClip)
	{

		bCanFire = false;
		float Delay = ReloadTime;
		if (ReloadAnim)
		{
			Delay = PlayCharAnimation(ReloadAnim);
		}
		if (MyPawn && MyPawn->bIsRunning)
		{
			MyPawn->StopRunning();
		}




		FTimerHandle TimerHandle_Reload;
		GetWorldTimerManager().SetTimer(TimerHandle_Reload, this, &AHWeaponBase::FinishReload, Delay, false);
	}
}

void AHWeaponBase::FinishReload()
{
	bCanFire = true;
	CurrentAmmo = CurrentAmmo - (MaxAmmoPerClip - CurrentAmmoinClip);
	CurrentAmmoinClip = FMath::Clamp(CurrentAmmoinClip + FMath::Min(CurrentAmmo, MaxAmmoPerClip), 0, MaxAmmoPerClip);


}

void AHWeaponBase::OnEquip()
{
	bCanFire = false;
	bIsEquiped = true;
	float Duration = PlayCharAnimation(EquipAnim);
	if (Duration <= 0.0f)
	{
		// failsafe
		Duration = 0.5f;
	}
	FTimerHandle TimerHandle_Equip;
	GetWorldTimerManager().SetTimer(TimerHandle_Equip, this, &AHWeaponBase::OnEquip_Finished, Duration, false);
}

void AHWeaponBase::OnUnequip()
{
	SetActorHiddenInGame(false);
	bIsEquiped = false;

}

void AHWeaponBase::OnEquip_Finished()
{
	bCanFire = true;
	if (CurrentAmmoinClip == 0 && bCanReload)
	{
		Reload();
	}
}

void AHWeaponBase::OnUnequip_Finished()
{
	bCanFire = false;
}



int32 AHWeaponBase::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

int32 AHWeaponBase::GetMaxAmmo() const
{
	return MaxAmmo;
}

void AHWeaponBase::SetOwningPawn(ABaseCharacter * NewOwner)
{
	if (MyPawn != NewOwner)
	{
		Instigator = NewOwner;
		MyPawn = NewOwner;
	}
}

FName AHWeaponBase::GetAttachementSocketName()
{
	if (bIsEquiped)
	{
		return EquipedSocketName;
	}
	else
	{
		return InventorySocketName;
	}
}

float AHWeaponBase::PlayCharAnimation(UAnimMontage* Animation, float InPlayRate)
{
	float Duration = 0.0f;
	if (MyPawn)
	{
		if (Animation)
		{
			Duration = MyPawn->PlayAnimMontage(Animation, InPlayRate);
		}
	}

	return Duration;
}

void AHWeaponBase::StopCharAnimation(UAnimMontage* Animation)
{
	if (MyPawn)
	{
		if (Animation)
		{
			MyPawn->StopAnimMontage(Animation);
		}
	}
}
