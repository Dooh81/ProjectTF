// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/CutSceneAnimNotifyState.h"

#include "Core/PlayerCharacter.h"

void UCutSceneAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                           float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (IsValid(Cast<APlayerCharacter>(MeshComp->GetOwner())))
	{
		Cast<APlayerCharacter>(MeshComp->GetOwner())->CutSceneProcess(m_bPause);
	}
}

void UCutSceneAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (IsValid(Cast<APlayerCharacter>(MeshComp->GetOwner())))
	{
		Cast<APlayerCharacter>(MeshComp->GetOwner())->ChangeCameraBattle();
	}
}
