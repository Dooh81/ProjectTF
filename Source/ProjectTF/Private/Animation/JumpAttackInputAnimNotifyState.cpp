// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/JumpAttackInputAnimNotifyState.h"
#include "Core/PlayerCharacter.h"

void UJumpAttackInputAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (IsValid(PlayerCharacter))
	{
		m_pOwner = PlayerCharacter;
	}
}

void UJumpAttackInputAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	if (IsValid(m_pOwner))
	{
		m_pOwner->JumpAttackCheck();
		m_pOwner->ActivateJumpAttack();
	}
}

void UJumpAttackInputAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	
	m_pOwner = nullptr;

}
