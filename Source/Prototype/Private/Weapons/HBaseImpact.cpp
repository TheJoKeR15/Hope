// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/HBaseImpact.h"


// Sets default values
AHBaseImpact::AHBaseImpact()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHBaseImpact::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHBaseImpact::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

