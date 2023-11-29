// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


enum class EChainDamagedType : uint8;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTTF_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetDefaultGravityScale() const {return fDefaultGravityScale;}
	float GetLauncherGravityScale(EChainDamagedType Type);

	FVector GetLauncherDistance(EChainDamagedType Type);
	int32 GetMaxJuggleCount() const {return  MaxJuggleCount;}
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combo", meta = (ToolTip = "GravityScale"))
	float fUpperLauncherGravityScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combo", meta = (ToolTip = "GravityScale"))
	float fMiddleLauncherGravityScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combo", meta = (ToolTip = "GravityScale"))
	float fLowerLauncherGravityScale;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combo", meta = (ToolTip = "GravityScale"))
	float fDefaultGravityScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combo", meta = (ToolTip = "Y: Forward Distance, Z: UpperDistance"))
	FVector UpperLauncherDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combo", meta = (ToolTip = "Y: Forward Distance, Z: UpperDistance"))
	FVector MiddleLauncherDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combo", meta = (ToolTip = "Y: Forward Distance, Z: UpperDistance"))
	FVector LowerLauncherDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combo", meta = (ToolTip = "MAX juggleCount"))
	int32 MaxJuggleCount;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera", meta = (ToolTip = "Camera CutScene BlendTime"))
	float fCutSceneCameraBlendTime;
	
	UFUNCTION(BlueprintCallable)
	void CameraShake();

	float GetMaxJuggleCount(){return MaxJuggleCount;}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf <class ULegacyCameraShake> CameraShakeSource;

	TArray<AActor*> Camera;
};
