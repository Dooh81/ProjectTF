// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ActiveDamageBoxAnimNotify.h"

#include "Core/ActionInputComponent.h"
#include "Core/HitBoxComponent.h"

UActiveDamageBoxAnimNotify::UActiveDamageBoxAnimNotify()
{
	FBoxStatus initBoxStatus;
	initBoxStatus.Center = FVector2D(0, 90);
	initBoxStatus.Extent = FVector2D(32, 90);
	BoxStatus.Add(initBoxStatus);
}

void UActiveDamageBoxAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	if (BoxStatus.Num() != 0)
	{
		if (IsValid(Cast<APlayerCharacter>(MeshComp->GetOwner())))
		{
			Cast<APlayerCharacter>(MeshComp->GetOwner())->ChangeDamageBoxTransform(DamageBoxType,BoxStatus);

		}
		else
		{
			for (int i = 0; i < BoxStatus.Num(); i++)
			{
				DrawDebugBox(MeshComp->GetWorld(),
					FVector(0.0f, BoxStatus[i].Center.X, BoxStatus[i].Center.Y),
					FVector(0.0f, BoxStatus[i].Extent.X, BoxStatus[i].Extent.Y),
					FColor::Green, false, 1.f, 0U, 3.f);
			}
		}
	}
}