// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTF_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UCharacterAnimInstance() {}

	void MontageBlendOut(class UAnimMontage* animmontage, bool trigger);
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly)
		bool bInAir;
	UPROPERTY(BlueprintReadOnly)
		bool bCrouch;

	class APlayerCharacter* PlayerCharacter;
};
