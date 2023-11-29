// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/CharInvincibleAnimNotifyState.h"
#include "Core/HitBoxComponent.h"


void UCharInvincibleAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	Owner = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (IsValid(Owner))
	{
		Owner->ActiveDamegeBox(EStateofDamageBox::E_Invincible);
	}
}


void UCharInvincibleAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UCharInvincibleAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	if (IsValid(Owner))
	{
		Owner->ActiveDamegeBox(DamageBoxType);
	}

}
