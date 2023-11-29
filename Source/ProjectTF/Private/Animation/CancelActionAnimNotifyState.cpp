// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/CancelActionAnimNotifyState.h"

#include "Core/ActionInputComponent.h"
#include "Core/PlayerCharacter.h"

void UCancelActionAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                               float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	isCancel = false;
}

void UCancelActionAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if(IsValid(PlayerCharacter))
	{
		if(PlayerCharacter->ActionInputComponent->GetCurrentAction()->GetData()->Cancel_Act == 1)
		{
			unsigned long ActionNumber = 0;
			ActionNumber = PlayerCharacter->ActionInputComponent->CancelActionCheck();
			if(ActionNumber != 0)
			{
				ActionNumber = PlayerCharacter->GetSerialNo() * 10000 + ActionNumber;
				PlayerCharacter->ActionInputComponent->ChangeAction(ActionNumber);
			}
		}
	}
}

void UCancelActionAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	
	
}
