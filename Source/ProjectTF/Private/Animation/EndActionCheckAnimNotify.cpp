// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/EndActionCheckAnimNotify.h"
#include "Core/PlayerCharacter.h"

void UEndActionCheckAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	if (IsValid(Cast<APlayerCharacter>(MeshComp->GetOwner())))
	{
		//UE_LOG(LogTemp, Warning, TEXT("EndActionNotify AnimName %s"), *Animation->GetName());
		Cast<APlayerCharacter>(MeshComp->GetOwner())->EndActionCheck();
	}
}