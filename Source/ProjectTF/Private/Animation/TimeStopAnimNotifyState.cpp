// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/TimeStopAnimNotifyState.h"
#include "Core/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

void UTimeStopAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	Owner = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (IsValid(Owner))
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.f);
		Owner->CustomTimeDilation = 1.f;
	}
}

void UTimeStopAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
}

void UTimeStopAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (IsValid(Owner))
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.f);
	}
}
