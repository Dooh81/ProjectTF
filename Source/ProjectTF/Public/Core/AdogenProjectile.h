// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/PlayerCharacter.h"
#include "AdogenProjectile.generated.h"

UCLASS()
class PROJECTTF_API AAdogenProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAdogenProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Blueprintreadwrite)
	APlayerCharacter* OwnerPlayer;
};
