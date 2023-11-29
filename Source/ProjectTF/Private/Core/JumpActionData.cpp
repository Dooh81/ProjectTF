// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/JumpActionData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Core/DataTableManager.h"

#define START "Start"
#define LOOP "Loop"
#define END "End"

// Jump Action Data 
JumpActionData::JumpActionData(ActionDataTable* dataTable, APlayerCharacter* OwnerPlayer, APlayerCharacter* EnemyPlayer, FOnMontageBlendingOutStarted blendOutDelegate, float RotateValue) :
	ActionData(dataTable, OwnerPlayer, EnemyPlayer, blendOutDelegate, RotateValue)
{

}

void JumpActionData::Enter()
{
	ActionData::Enter();

	FString ani = m_pActionDataTable->AnimName.c_str();
	ani += "_Montage";

	FString CharacterName = m_pCharacterDataTable->Name.c_str();

	FString URL = "/Game/ArtWork/Characters/" + CharacterName + "/Animations/Montage/" + ani + "." + ani;

	m_curAnim = LoadObject<UAnimMontage>(nullptr, *URL);



	if (m_curAnim != nullptr)
	{
		m_pOwner->GetMesh()->GetAnimInstance()->Montage_Play(m_curAnim);
		m_pOwner->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(m_MontageBlendingOutStartedDelegate, m_curAnim);
	}

	//Rotate
	if (m_pActionDataTable->RotateType == (int)ActionData::ENTER_ROTATE_TYPE)
	{
		RotateUpdate();
	}

	m_jumpActionState = EJumpActionState::E_Start;
	
}

void JumpActionData::Exit()
{
	ActionData::Exit();
}



void JumpActionData::MontageBlendOut(class UAnimMontage* animmontage, bool trigger)
{
	ActionData::MontageBlendOut(animmontage, trigger);

	//UE_LOG(LogTemp, Warning, TEXT("Jump BlendOut"));

	if (m_jumpActionState != EJumpActionState::E_Start)
	{
		return;
	}

	m_jumpActionState = EJumpActionState::E_Loop;
}

void JumpActionData::Tick(float DeltaTime)
{
	ActionData::Tick(DeltaTime);
	
	if (m_jumpActionState != EJumpActionState::E_Loop)
	{
		return;
	}

	if (m_pOwner->GetCharacterMovement()->IsFalling() == false)
	{
		/*UCapsuleComponent* Capsule = m_pOwner->GetCapsuleComponent();
		Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

		UE_LOG(LogTemp, Warning, TEXT("ECR_Block"));
		m_jumpActionState = E_End;
		m_pOwner->EndActionAnimation();*/
	}
}