// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Core/PlayerCharacter.h"
#include "Core/HitBoxComponent.h"
#include "ActiveHitBoxAnimNotifyState.generated.h"

/**
 * 
 */


UCLASS()
class PROJECTTF_API UActiveHitBoxAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UActiveHitBoxAnimNotifyState();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	class APlayerCharacter* Owner;
public:

	UPROPERTY(EditAnywhere,BlueprintReadwrite)
		TArray<FBoxStatus> BoxStatus;

	UPROPERTY(EditAnywhere)
		EUseFinishState FinishState;
};
