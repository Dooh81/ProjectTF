// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/StartLauchCharacterAnimNotify.h"
#include "Core/PlayerCharacter.h"

void UStartLauchCharacterAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	APlayerCharacter* Owner = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (Owner != nullptr)
	{
		FVector MoveMentLaucherVector = m_MoveVector;
		if (Owner->GetActionRotateValue() < 0)
		{
			MoveMentLaucherVector.Y = -MoveMentLaucherVector.Y;
		}
		Owner->LaunchCharacter(MoveMentLaucherVector, true, true);
	}
}
