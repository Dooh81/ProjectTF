// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/StartActionCheckAnimNotify.h"
#include "Core/PlayerCharacter.h"

void UStartActionCheckAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	if (IsValid(Cast<APlayerCharacter>(MeshComp->GetOwner())))
	{
		Cast<APlayerCharacter>(MeshComp->GetOwner())->StartActionCheck();
	}
}