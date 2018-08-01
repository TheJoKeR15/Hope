// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/HBaseProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "HWeaponBase.h"
#include "HBaseImpact.h"


// Sets default values
AHBaseProjectile::AHBaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent =CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	Mesh->SetupAttachment(CollisionComp);
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	CollisionComp->bShouldUpdatePhysicsVolume = true;
	CollisionComp->bCheckAsyncSceneOnMove = false;
	CollisionComp->SetCanEverAffectNavigation(false);
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));

}

// Called when the game starts or when spawned
void AHBaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHBaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHBaseProjectile::SpawnIpmactEffects(TSubclassOf<AHBaseImpact> ImpactClassToSpawn,FHitResult &Hit)
{
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(Hit.ImpactPoint);
	SpawnTransform.SetRotation(Hit.ImpactNormal.Rotation().Quaternion());
	AActor* AImpact = GetWorld()->SpawnActor<AActor>(ImpactClassToSpawn,SpawnTransform);
}

void AHBaseProjectile::SetOwnerWeapon(AHWeaponBase * OwnerWeapon)
{
	if (OwnerWeapon && OwnerWeapon != MyWeapon)
	{
		MyWeapon = OwnerWeapon;
	}
}

void AHBaseProjectile::SetOwnerPawn(ABaseCharacter * OwnerPawn)
{
	if (OwnerPawn && OwnerPawn != MyPawn)
	{
		MyPawn = OwnerPawn;
	}
}

