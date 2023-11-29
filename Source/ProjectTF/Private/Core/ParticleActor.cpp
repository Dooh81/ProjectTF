// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ParticleActor.h"

#include "Particles/ParticleSystemComponent.h"

// Sets default values
AParticleActor::AParticleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default"));
	DefaultSceneComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SetRootComponent(DefaultSceneComponent);
	FXComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("fx"));
	FXComponent->AttachToComponent(DefaultSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AParticleActor::DestroyParticleActor()
{
	Destroy();
}

// Called when the game starts or when spawned
void AParticleActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AParticleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

