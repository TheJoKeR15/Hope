// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HWeaponBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UParticleSystem;
class ABaseCharacter;
class AHBaseImpact;
class UAnimMontage;
class AHBaseProjectile;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Rifle,
	Pistol,
	Burst,
	Snipe,
};

UCLASS()
class PROTOTYPE_API AHWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHWeaponBase();
	/* ANIMATIONS */

	/* Animation that occure at the weapon for Firing */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations|Weapon")
		UAnimMontage* WeapFireAnim;

	/* Animation that occure at the weapon for Reloading */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations|Weapon")
		UAnimMontage* WeapReloadAnim;

	/* Animation that occure at the weapon for Equiping */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations|Weapon")
		UAnimMontage* WeapEquipAnim;

	/* Animation that occure at the Character for Firnig */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations|Character")
		UAnimMontage* FireAnim;

	/* Animation that occure at the Character for Reloading  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations|Character")
		UAnimMontage* ReloadAnim;

	/* Animation that occure at the Character for Equiping */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations|Character")
		UAnimMontage* EquipAnim;

	/* Animation that occure at the Character for Firing while aiming */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations|Character")
		UAnimMontage* AimFireAnim;

	/* AMMO */

	/*Max Ammo in one clip*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Ammo")
		int32 MaxAmmoPerClip;

	/*Max ammo the character can carry for this weapon*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Ammo")
		int32 MaxAmmo;

	/* the current ammo the weapon have */
	UPROPERTY(BlueprintReadWrite)
		int32 CurrentAmmo;

	UPROPERTY(BlueprintReadWrite)
		int32 CurrentAmmoinClip;

	/* default reload time when there is no reload anim */
	UPROPERTY(EditDefaultsOnly, Category = "Config|Ammo")
		float ReloadTime;

	/* CONFIGURATION */

	/* The type of the weapon  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
		EWeaponType WeaponType;

	/* if the player can drop this weapon */
	UPROPERTY(EditDefaultsOnly, Category = "Config")
		bool bCanBeDroped;

	/* The Max value that the spread can reach in degres */
	UPROPERTY(EditDefaultsOnly, Category = "Config ")
		float MaxSpread;

	/* the spread added to the weapon for each shot in degres */
	UPROPERTY(EditDefaultsOnly, Category = "Config")
		float Spread;

	/* the spread added to the weapon for each shot in degres */
	UPROPERTY(EditDefaultsOnly, Category = "Config")
		float TargetingFOV;

	/* The Spread decreasing interp speed */
	UPROPERTY(EditDefaultsOnly, Category = "Config")
		float SpreadDecreaseInterp;

	

	/* the current Horizontal and vertical spread */
	UPROPERTY(BlueprintReadOnly)
		float SpreadV;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config ")
		FName WeaponName;

	/* The Maximum Distance the weapon can hit */
	UPROPERTY(EditDefaultsOnly, Category = "Config")
		float WeaponRange;

	/* DAMMAGE */

	/* The damage that is caused by one bullet at 1m (the actual damage is calculated based on the distance) */
	UPROPERTY(EditDefaultsOnly, Category = "Config")
		float Damage;

	/* The amount of armor penetration value from 0 to 1 where : 0-No armor penetration 1- 100% armor penetration */
	UPROPERTY(EditDefaultsOnly, Category = "Config")
		float ArmorPenetration;

	/* The max distance this bulet can penetrate */
	UPROPERTY(EditDefaultsOnly, Category = "Config")
		float MaxPenetration;

	/* the amount the bullet can penetrate walls */
	UPROPERTY(EditDefaultsOnly, Category = "Config")
		float BulletPenetration;

	/* The class of the Dmage type */
	UPROPERTY(EditDefaultsOnly, Category = "Config")
		TSubclassOf<UDamageType> DamageTypeClass;

	/* The class of the Dmage type */
	UPROPERTY(EditDefaultsOnly, Category = "Config")
		TSubclassOf<AHBaseImpact> ImpactEffectClass;

	/* SOUNDS */

	/* The Sound used when the weapon is firing */
	UPROPERTY(EditDefaultsOnly, Category = "Config|Sounds")
		USoundBase* FireSound;

	/* The sound used when there is no bullets left */
	UPROPERTY(EditDefaultsOnly, Category = "Config|Sounds")
		USoundBase* EmptySound;

	/* The sound used when the weapon is being equiped */
	UPROPERTY(EditDefaultsOnly, Category = "Config|Sounds")
		USoundBase* EquipSound;

	/* The Sound used when the sound is being unequiped */
	UPROPERTY(EditDefaultsOnly, Category = "Config|Sounds")
		USoundBase* UnequipSound;


	/* Effects */

	/* The particle system for the muzle flash effect */
	UPROPERTY(EditDefaultsOnly, Category = "Config|Effects")
		UParticleSystem* MuzzleFlashEffect;

	/* The pariicle system used for the trail effect */
	UPROPERTY(EditDefaultsOnly, Category = "Config|Effects")
		UParticleSystem* TrailEffect;

	/* The name of the muzzle flash Socket */
	UPROPERTY(EditDefaultsOnly, Category = "Config|Effects")
		FName MuzzleFlashSocketName;

	/* The Intensity of the Fire Camera shake */
	UPROPERTY(EditDefaultsOnly, Category = "Config|Effects")
		float FireShakeIntensity;
	UPROPERTY(EditDefaultsOnly, Category = "Config|Effects")
		TSubclassOf<UCameraShake> FireCamShake;
	UPROPERTY(EditDefaultsOnly, Category = "Config")
		TSubclassOf<AHBaseProjectile> BulletClass;
	UPROPERTY(BlueprintReadWrite, Category = "Config")
		bool bCanFire;
	UPROPERTY(BlueprintReadWrite, Category = "Config")
		bool bCanReload;
	UPROPERTY(BlueprintReadWrite, Category = "Config")
		bool bIsEquiped;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
		FName EquipedSocketName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
		FName InventorySocketName;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* MeshComp;

	class ABaseCharacter* MyPawn;

	/* The ACTUAL firing logic hapens here !! */

	UFUNCTION(BlueprintCallable, Category = "Config")
		virtual void Fire();


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Call the Fire() function all sub-weapon types should ovveride this function
	This is useful for automatique weapons*/
	virtual void StartFire();

	/* stop firing this is usful for autimatic fire */
	virtual void StopFire();


	/* here we play the cosmetic effects (sounds ,muzzel flash ...) , Useful for multiplayer implementation !!*/
	virtual void PlayEffects(FVector ImpactPoint);

	/* Reloading the weapon */
	virtual void Reload();

	virtual void FinishReload();

	/* this is caled when equiping the weapon , play sound and anims ... */
	virtual void OnEquip();

	/* Called when the caracter is puting the weapon down */
	virtual void OnUnequip();

	/* Called when finished equiping  */
	virtual void OnEquip_Finished();

	/* Called when the caracter is puting the weapon down */
	virtual void OnUnequip_Finished();


	/* Get the current Ammo this weapon have right now */
	UFUNCTION(BlueprintCallable, Category = "Config")
		int32 GetCurrentAmmo() const;

	/* Get the max ammo this weapon can have */
	UFUNCTION(BlueprintCallable, Category = "Config")
		int32 GetMaxAmmo() const;

	/* We set the owning pzwn for this weapon */
	UFUNCTION(BlueprintCallable, Category = "Config")
		void SetOwningPawn(ABaseCharacter* NewOwner);

	/* Get the socket name dependig if we are equiped or not */
	UFUNCTION(BlueprintCallable, Category = "Config")
		FName GetAttachementSocketName();

	/* Play the character Animation */
	float PlayCharAnimation(UAnimMontage* Animation, float InPlayRate = 1.f);

	/** stop playing Character animations */
	void StopCharAnimation(UAnimMontage* Animation);

	virtual USceneComponent* GetMesh();

	// Add the delta to the spread 
	virtual void AddSpread(float Delta);

	virtual void SetSpread(float NewSpread);

	virtual void OnStartTargeting();

	virtual void OnStopTargeting();

};
