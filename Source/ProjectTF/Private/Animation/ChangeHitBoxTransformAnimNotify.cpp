// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ChangeHitBoxTransformAnimNotify.h"
#include "Core/PlayerCharacter.h"
#include "Core/HitBoxComponent.h"


void UChangeHitBoxTransformAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	APlayerCharacter* Owner = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (BoxStatus.Num() != 0)
	{
		if (IsValid(Owner))
		{
			Owner->ChangeHitBoxTransform(BoxStatus);
		}
		else
		{
			for (int i = 0; i < BoxStatus.Num(); i++)
			{
				DrawDebugBox(MeshComp->GetWorld(),
					FVector(0.0f, BoxStatus[i].Center.X, BoxStatus[i].Center.Y),
					FVector(32.0f, BoxStatus[i].Extent.X, BoxStatus[i].Extent.Y),
					FColor::Red, false, 1.f, 0U, 3.f);
			}

		}

	}
}