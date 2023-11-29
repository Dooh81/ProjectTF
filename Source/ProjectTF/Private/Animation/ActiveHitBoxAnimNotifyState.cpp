// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ActiveHitBoxAnimNotifyState.h"
#include "Core/ActionInputComponent.h"


UActiveHitBoxAnimNotifyState::UActiveHitBoxAnimNotifyState()
{
	BoxStatus.Add(FBoxStatus());
}

void UActiveHitBoxAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	Owner = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (BoxStatus.Num() != 0)
	{
		if (IsValid(Owner))
		{
			Owner->ActiveHitBoxByIndex(BoxStatus, FinishState);

		}
		else
		{
			for (int i = 0; i < BoxStatus.Num(); i++)
			{
				DrawDebugBox(MeshComp->GetWorld(), 
					FVector(0.0f, BoxStatus[i].Center.X, BoxStatus[i].Center.Y), 
					FVector(0.0f, BoxStatus[i].Extent.X, BoxStatus[i].Extent.Y),
					FColor::Red, false, 1.f, 0U, 3.f);
			}

		}
	}
	else
	{
		if (IsValid(Owner))
		{
			Owner->ActiveFinishState(FinishState);
		}
	}
}


void UActiveHitBoxAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	if(IsValid(Owner))
	{
		if(Owner->AttackerState == EAttackerState::E_Attack)
		{
			//UE_LOG(LogTemp,Warning,TEXT("Attack Success"));
		}
	}
}

void UActiveHitBoxAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	if (IsValid(Owner))
	{
		Owner->DeActiveHitBox();
	}

}
