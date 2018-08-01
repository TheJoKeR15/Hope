// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HBaseProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class AHWeaponBase;
class ABaseCharacter;
class AHBaseImpact;

UCLASS()
class PROTOTYPE_API AHBaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHBaseProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UProjectileMovementComponent* ProjectileMovementComp;
	
	UPROPERTY(BlueprintReadOnly,Category = "Config")
	ABaseCharacter* MyPawn;

	UPROPERTY(BlueprintReadOnly, Category = "Config")
	AHWeaponBase* MyWeapon;

	UPROPERTY(VisibleAnywhere, Category = "Config")
	TSubclassOf<AHBaseImpact> ImpactClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//Spawn the Impact at the Given Hit.ImpactPoint And The Hit.ImpactNormal Rotation.
	UFUNCTION(BlueprintCallable,Category ="Bullet")
	virtual void SpawnIpmactEffects(TSubclassOf<AHBaseImpact> ImpactClassToSpawn, FHitResult &Hit);

	// Set the weapon owning this bullet
	UFUNCTION(BlueprintCallable, Category = "Net")
	virtual void SetOwnerWeapon(AHWeaponBase* OwnerWeapon);
	// Set the owner of this bullet (Pawn)
	UFUNCTION(BlueprintCallable, Category = "Net")
	virtual void SetOwnerPawn(ABaseCharacter* OwnerPawn);
};
