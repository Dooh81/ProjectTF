// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TokenFighterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTF_API UTokenFighterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMirror = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float fMirrorBlendTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFalling = false;
};
