// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/JumpEndAnimNotify.h"
#include "Core/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"

void UJumpEndAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (PlayerCharacter)
	{
		//PlayerCharacter->fMirrorBlendTime = 0.5f;
		//PlayerCharacter->ActivePushBox(true);
		 UCapsuleComponent* Capsule = PlayerCharacter->GetCapsuleComponent();
		 Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

		 PlayerCharacter->PushStrongBoxComponents[0]->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel7);
		 PlayerCharacter->PushStrongBoxComponents[0]->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel7, ECollisionResponse::ECR_Overlap);
		 PlayerCharacter->PushStrongBoxComponents[0]->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel8, ECollisionResponse::ECR_Ignore);
	}
}
