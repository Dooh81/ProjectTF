// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/JumpStartNotify.h"
#include "Components/CapsuleComponent.h"
#include "Core/PlayerCharacter.h"

void UJumpStartNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	APlayerCharacter* owner = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (owner != nullptr)
	{
		
		owner->JumpStart(JumpState);

		UCapsuleComponent* Capsule = owner->GetCapsuleComponent();
		Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		owner->PushStrongBoxComponents[0]->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel8);
		owner->PushStrongBoxComponents[0]->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel7, ECollisionResponse::ECR_Ignore);
		owner->PushStrongBoxComponents[0]->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel8, ECollisionResponse::ECR_Overlap);

		//owner->ActivePushBox(false);
		owner->fMirrorBlendTime = 0.0f;
		UE_LOG(LogTemp, Warning, TEXT("ECR_Ignore"));
	}
}
