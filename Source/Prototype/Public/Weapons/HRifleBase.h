// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HWeaponBase.h"
#include "HRifleBase.generated.h"



UCLASS()
class PROTOTYPE_API AHRifleBase : public AHWeaponBase
{
	GENERATED_BODY()
	

public:
	AHRifleBase();

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	float RPM;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	/* time betwen each shot = 60/RPM */
	float TimeBetweenShots;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	/* The recoil of the weapon */
	float Recoil;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	/* Max recoilof the weapon */
	float MaxRecoil;

	float RecoilH;

	float RecoilV;

	float LastFireTime;

protected:

	FTimerHandle FTimerHandle_Firing;

	virtual void Fire() override;

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void Reload() override;

	virtual void StartFire() override;

	virtual void StopFire() override;

	virtual void PlayEffects(FVector ImpactPoint) override;

};
