// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ChangeHitBoxTransformAnimNotify.generated.h"
/**
 * 
 */
UCLASS()
class PROJECTTF_API UChangeHitBoxTransformAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		TArray<struct FBoxStatus> BoxStatus;
};
