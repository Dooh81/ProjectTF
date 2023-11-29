// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/CharacterAnimInstance.h"
#include "Core/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCharacterAnimInstance::NativeBeginPlay()
{
	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	FOnMontageBlendingOutStarted blendOutDelegate;
	blendOutDelegate.BindUObject(this, &UCharacterAnimInstance::MontageBlendOut);

}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (IsValid(PlayerCharacter))
	{
		bInAir = PlayerCharacter->GetCharacterMovement()->IsFalling();
		bCrouch = PlayerCharacter->GetCharacterMovement()->IsCrouching();

	}
}

void UCharacterAnimInstance::MontageBlendOut(class UAnimMontage* animmontage, bool trigger)
{

}
