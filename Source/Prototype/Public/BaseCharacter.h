// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class USphereComponent;
class UCameraComponent;
class USpringArmComponent;
class AHWeaponBase;

UCLASS()
class PROTOTYPE_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	UPROPERTY(EditDefaultsOnly, Category = "Config")
		float MouseSensitivity;

	/*									CHEATS															 */
	// these bools should be modified via the consol and chect if the cheats are actiated in the GameMod

	/* the playen cant recive damage and cant die */
	UPROPERTY(EditDefaultsOnly, Category = "Cheats")
		bool bGodMod;

	/** inifite ammo for reloads */
	UPROPERTY(EditDefaultsOnly, Category = "Cheats")
		bool bInfiniteAmmo;

	/** infinite ammo in clip, no reload required */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cheats")
		bool bInfiniteClip;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
		TSubclassOf<AHWeaponBase> StartingWeaponClass;
	UPROPERTY(BlueprintReadOnly, Category = "Config")
		AHWeaponBase* CurrentWeapon;

	UPROPERTY(BluePrintReadOnly)
		TArray<AHWeaponBase*> Inventory;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
		int32 CurrentInventoryIndex;

	UPROPERTY(BlueprintReadWrite, Category = "Config")
		bool bIsTargeting;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
		float TargetingSpeedModif;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
		float RunningSpeedModif;

	/* The Min value that the spread can reach when walking in degres */
	UPROPERTY(EditDefaultsOnly, Category = "Config ")
		float MinWalkingSpread;

	/* The Min value that the spread can reach when Failling (Jamping) in degres */
	UPROPERTY(EditDefaultsOnly, Category = "Config ")
		float MinFallingSpread;

	/* the spread added to the weapon for each shot in degres */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float TargetingMouseSensitivity;

	/* the modifier for when wea are targeting and increasing acuracy */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float TargetingSpreadModif;
	/* the modifier for when wea are Crouching and increasing acuracy */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float CrouchingSpreadModif;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
		bool bCanRun;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
		bool bIsFiring;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
		bool bIsRunning;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
		bool bIsReloading;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
		bool bIsJumping;

	float DefaultMaxSpeed;
	float DefaultFOV;


protected:

	void MoveForward(float Value);

	void MoveRight(float Value);

	void LookUp(float Value);

	void LookRight(float Value);

	UFUNCTION(BlueprintNativeEvent, Category = "Movements")
		void OnJump();
	void OnJump_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Movements")
		void OnStopJump();
	void OnStopJump_Implementation();

	void BeginCrouch();

	void EndCrouch();

	UFUNCTION(BlueprintPure, Category = "Movements")
		bool CanUncrouch();

	UFUNCTION(BlueprintCallable, Category = "Config")
		void SetMouseSensitivity(float newSensitivity);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USpringArmComponent* MeshLagComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USpringArmComponent* CamSpringComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UArrowComponent* DropForceComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		FVector DropForce;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float DropForceStrenght;




public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void SpawnWeapon(TSubclassOf<class AHWeaponBase> WeaponClass);

	void StartWeaponFire();

	void StopWeaponFire();

	void OnReload();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void AttachWeapon(AHWeaponBase* Weapon);

	virtual FVector GetPawnViewLocation() const override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		virtual bool AddToInventory(AHWeaponBase* Item, int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		virtual int32 GetFirstEmptySlot();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		virtual AHWeaponBase* GetFromInventory(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		virtual bool RemoveFromInventory(AHWeaponBase* Weapon);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		virtual void EquipWeapon(AHWeaponBase* Weapon);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		virtual void EquipNext();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		virtual void EquipLast();

	/** play anim montage */
	virtual float PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None) override;

	/** stop playing montage */
	virtual void StopAnimMontage(class UAnimMontage* AnimMontage) override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		virtual void OnDropWeapon(AHWeaponBase* Weapon);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		virtual void DropWeapon();

	virtual void SetTargeting(bool NewTargeting);

	bool GetTargeting();

	virtual void OnBeginTargeting();

	virtual void OnEndTargeting();

	virtual void SetMaxSpeed(float SpeedModifier);

	virtual void StartRunning();

	virtual void StopRunning();


};

