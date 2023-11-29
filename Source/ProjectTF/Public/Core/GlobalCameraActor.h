// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GlobalCameraActor.generated.h"

UCLASS()
class PROJECTTF_API AGlobalCameraActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGlobalCameraActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
		void SetPlayers();

	void ChangeFOV();
	void ChangeFOVIn();

	void ChangeFOVOut();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EndFOV = 40.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LerpTime = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FirstLerpTime = 0.05f;

	const float DefaultFOV = 60.0f;
	float ElapsedTime;

	FTimerHandle FOVTimerHandle;
	
	UPROPERTY(BlueprintReadwrite)
		APawn* Player01;
	UPROPERTY(BlueprintReadwrite)
		APawn* Player02;

};
