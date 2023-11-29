// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "StartLauchCharacterAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTF_API UStartLauchCharacterAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void  Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

	UPROPERTY(EditAnywhere)
	FVector m_MoveVector;
};
