// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/MovementCheckAnimNotifyState.h"

UMovementCheckAnimNotifyState::UMovementCheckAnimNotifyState()
{
}

void UMovementCheckAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	Owner = Cast<APlayerCharacter>(MeshComp->GetOwner());


	if (Owner != nullptr)
	{
		if(Owner->CustomTimeDilation != 1.f)
		{
			return;
		}
		
		StartLocation = Owner->GetActorLocation();
		TotalDistance = TotalDuration * MoveDistancePerSec; // 전체 이동 거리
		TotalTime = TotalDuration;
		
		Owner->GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle,
			FTimerDelegate::CreateLambda([this]()-> void
				{
					if (Owner != nullptr)
					{
						FVector forward = Owner->GetActionRotateValue() * -Owner->GetActorRightVector();
						forward.Z = 0.0f;
						forward.Normalize();
						FVector TargetLocation = StartLocation + forward * TotalDistance;
						Owner->SetActorLocation(TargetLocation, true);
					}
				}
			)
			, TotalDuration
			, false
			);
	}
}

void UMovementCheckAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	Owner = Cast<APlayerCharacter>(MeshComp->GetOwner());
	
	if (Owner != nullptr)
	{
		if(Owner->CustomTimeDilation != 1.f)
		{
			return;
		}
		
		if (Owner->GetWorld()->GetTimerManager().IsTimerActive(MoveTimerHandle))
		{
			float ElapsedTime = Owner->GetWorld()->GetTimerManager().GetTimerElapsed(MoveTimerHandle);
			float Ratio = ElapsedTime / TotalTime;

			FVector forward = Owner->GetActionRotateValue() * -Owner->GetActorRightVector();
			forward.Z = 0.0f;
			forward.Normalize();
			
			FVector TargetLocation = StartLocation + forward * Ratio * TotalDistance;
			
			Owner->SetActorLocation(TargetLocation, true);
		}
	}
}

void UMovementCheckAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	Owner = Cast<APlayerCharacter>(MeshComp->GetOwner());

	if (Owner != nullptr)
	{
		Owner->GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
	}
}
