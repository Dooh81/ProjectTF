// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CheckMoveValueAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTF_API UCheckMoveValueAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	UCheckMoveValueAnimNotify();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	class APlayerCharacter* Owner;
	
public:
	UPROPERTY(EditAnywhere)
		float MoveDistance;

	UPROPERTY(EditAnywhere)
		float Duration;

};
