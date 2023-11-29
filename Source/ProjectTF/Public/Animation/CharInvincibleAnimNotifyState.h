// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Core/PlayerCharacter.h"
#include "CharInvincibleAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTF_API UCharInvincibleAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	class APlayerCharacter* Owner;
public:
	UPROPERTY(EditAnywhere)
		EStateofDamageBox DamageBoxType = EStateofDamageBox::E_Idle;


};
