// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/CheckMoveValueAnimNotify.h"
#include "Core/PlayerCharacter.h"

UCheckMoveValueAnimNotify::UCheckMoveValueAnimNotify()
{
	
}

void UCheckMoveValueAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
	Owner = Cast<APlayerCharacter>(MeshComp->GetOwner());

	if (IsValid(Owner))
	{
		//Owner->AddMovement(MoveDistance, Duration);
		UE_LOG(LogTemp,Warning,TEXT("Not Used"));
	}
	
	
}
