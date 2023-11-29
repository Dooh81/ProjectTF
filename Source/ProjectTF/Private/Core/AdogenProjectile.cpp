// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AdogenProjectile.h"

// Sets default values
AAdogenProjectile::AAdogenProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAdogenProjectile::BeginPlay()
{
	Super::BeginPlay();
	OwnerPlayer = Cast<APlayerCharacter>(GetOwner());
}

// Called every frame
void AAdogenProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

