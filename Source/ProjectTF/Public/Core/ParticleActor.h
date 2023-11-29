// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ParticleActor.generated.h"

UCLASS()
class PROJECTTF_API AParticleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AParticleActor();
	void DestroyParticleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* DefaultSceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* FXComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
