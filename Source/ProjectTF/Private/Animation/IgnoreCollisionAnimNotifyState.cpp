// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/IgnoreCollisionAnimNotifyState.h"
#include "Components/CapsuleComponent.h"

#include "Core/PlayerCharacter.h"

void UIgnoreCollisionAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                  float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if(Player)
	{
		UCapsuleComponent* Capsule = Player->GetCapsuleComponent();
		Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	}
}

void UIgnoreCollisionAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if(Player)
	{
		UCapsuleComponent* Capsule = Player->GetCapsuleComponent();
		Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	}
}
