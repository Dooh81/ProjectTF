// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ActivePushBoxAnimNotifyState.h"


void UActivePushBoxAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	Owner = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (IsValid(Owner))
	{
		//Owner->ChangePushBox(BoxStatus);

	}
	else
	{
		DrawDebugBox(MeshComp->GetWorld(),
			FVector(0.0f, BoxStatus.Center.X, BoxStatus.Center.Y),
			FVector(32.0f, BoxStatus.Extent.X, BoxStatus.Extent.Y),
			FColor::Black, false, 1.f, 0U, 3.f);

	}
}

void UActivePushBoxAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

}

void UActivePushBoxAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	if (IsValid(Owner))
	{
		Owner->ActivePushBox(false);
	}
}
