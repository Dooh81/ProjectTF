// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Core/PlayerCharacter.h"
#include "ActiveDamageBoxAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTF_API UActiveDamageBoxAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	UActiveDamageBoxAnimNotify();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
		
	UPROPERTY(EditAnywhere)
		EStateofDamageBox DamageBoxType;

	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		TArray<FBoxStatus> BoxStatus;

};
