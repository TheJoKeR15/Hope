// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/HRifleBase.h"
#include "BaseCharacter.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Prototype.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

AHRifleBase::AHRifleBase()
{
	RPM = 600.0f;
	CurrentAmmoinClip = MaxAmmoPerClip;
	CurrentAmmo = MaxAmmo;

	SpreadV = FMath::DegreesToRadians(5.f);

	WeaponType = EWeaponType::Rifle;

}



void AHRifleBase::Tick(float DeltaTime)
{

}

void AHRifleBase::Reload()
{
	GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::White, TEXT("Reloading"));
	Super::Reload();
}

void AHRifleBase::StartFire()
{
	UE_LOG(LogTemp, Warning, TEXT("Starting Fire"))
		float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);
		GetWorldTimerManager().SetTimer(FTimerHandle_Firing, this, &AHRifleBase::Fire, TimeBetweenShots, true, FirstDelay);
}

void AHRifleBase::StopFire()
{
	MyPawn->bIsFiring = false;
	GetWorldTimerManager().ClearTimer(FTimerHandle_Firing);
}

void AHRifleBase::PlayEffects(FVector ImpactPoint)
{
	Super::PlayEffects(ImpactPoint);
}


void AHRifleBase::Fire()
{
	GLog->Log(TEXT("Fired !"));
	Super::Fire();
	LastFireTime = GetWorld()->TimeSeconds;
	AddSpread(Spread);
}

void AHRifleBase::BeginPlay()
{
	TimeBetweenShots = 60.0f / RPM;

	Super::BeginPlay();
	if (MyPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("The Owner of this weapon is %s"), *MyPawn->GetName());
	}
}
