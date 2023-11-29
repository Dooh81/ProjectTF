// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/CameraShakeAnimNotifyState.h"
#include "Core/CCommonController.h"
#include "Core/PlayerCharacter.h"

void UCameraShakeAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (PlayerCharacter)
	{
		m_pOwner = PlayerCharacter;

		
		float scale = 1.0f;

		ACCommonController* PlayerController = Cast<ACCommonController>(m_pOwner->GetController());
		PlayerController->ClientStartCameraShake(CameraShakeBase, scale);
	}
}

void UCameraShakeAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	
}

void UCameraShakeAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

}
