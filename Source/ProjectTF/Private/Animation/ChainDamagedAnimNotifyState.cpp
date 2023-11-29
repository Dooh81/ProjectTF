// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ChainDamagedAnimNotifyState.h"

#include "Components/CapsuleComponent.h"
#include "Core/ActionInputComponent.h"
#include "Core/CombatComponent.h"
#include "Core/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UChainDamagedAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                               float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	bOnes = false;
	
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (PlayerCharacter)
	{
		m_pOwner = PlayerCharacter;
		if(m_pOwner)
		{
			//m_pOwner->HitterState = EHitterState::E_Groggy;
			UCapsuleComponent* Capsule = m_pOwner->GetCapsuleComponent();
			if(ChainDamagedType != EChainDamagedType::E_Middle)
			{
				Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
			}
		}
	}
}

void UChainDamagedAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if(MeshComp !=nullptr && MeshComp->GetOwner() != nullptr && m_pOwner !=nullptr)
	{
		if(ChainDamagedType != EChainDamagedType::E_Middle && !m_pOwner->GetCharacterMovement()->IsFalling())
		{
			bOnes = true;
			MeshComp->GetAnimInstance()->Montage_JumpToSection(FName("End"));
			m_pOwner->GetCharacterMovement()->GravityScale = m_pOwner->CombatComponent->GetDefaultGravityScale();
			UCapsuleComponent* Capsule = m_pOwner->GetCapsuleComponent();
			Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

			m_pOwner->ActionInputComponent->GetCurrentAction()->GetData()->NextActionNo = m_pOwner->GetSerialNo() * 10000 + 13;
			m_pOwner->ActiveDamegeBox(EStateofDamageBox::E_Invincible);

			m_pOwner->ActionInputComponent->SetDefaultLowAttack();
		}
		
		if(m_pOwner->TFPlayerState->GetCurrentHitState() != ETFHitState::E_Juggle && !bOnes)
		{
			bOnes = true;
			MeshComp->GetAnimInstance()->Montage_JumpToSection(FName("End"));
			m_pOwner->GetCharacterMovement()->GravityScale = m_pOwner->CombatComponent->GetDefaultGravityScale();
			UCapsuleComponent* Capsule = m_pOwner->GetCapsuleComponent();
			Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
			if(ChainDamagedType != EChainDamagedType::E_Middle)
			{
				m_pOwner->ActionInputComponent->GetCurrentAction()->GetData()->NextActionNo = m_pOwner->GetSerialNo() * 10000 + 13;
			}
			else
			{
				m_pOwner->ActionInputComponent->GetCurrentAction()->GetData()->NextActionNo = m_pOwner->GetSerialNo() * 10000 + 12;
			}
			m_pOwner->ActiveDamegeBox(EStateofDamageBox::E_Invincible);
			m_pOwner->ActionInputComponent->SetDefaultLowAttack();
		}

		if(m_pOwner->TFPlayerState->GetHitDetailState() == ETFHitDetailState::E_Chain_Juggle_Finish && !bOnes)
		{
			bOnes = true;
			MeshComp->GetAnimInstance()->Montage_JumpToSection(FName("Finish"));
			m_pOwner->GetCharacterMovement()->GravityScale = m_pOwner->CombatComponent->GetDefaultGravityScale();
			UCapsuleComponent* Capsule = m_pOwner->GetCapsuleComponent();
			Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
			
			m_pOwner->ActionInputComponent->GetCurrentAction()->GetData()->NextActionNo = m_pOwner->GetSerialNo() * 10000 + 12;
			m_pOwner->ActiveDamegeBox(EStateofDamageBox::E_Invincible);
			m_pOwner->ActionInputComponent->SetDefaultLowAttack();
		}
		
		if(m_pOwner->TFPlayerState->GetHitDetailState() == ETFHitDetailState::E_Chain_Juggle_Hit && !bOnes)
		{
			bOnes = true;
			MeshComp->GetAnimInstance()->Montage_JumpToSection(FName("Hit"));

			switch (ChainDamagedType)
			{
			case EChainDamagedType::E_None:
				UE_LOG(LogTemp,Error,TEXT("Others"));
				break;
			case EChainDamagedType::E_Upper:
				m_pOwner->LaunchCharacter(FVector(HitChainDistance.X, m_pOwner->GetActionRotateValue() * HitChainDistance.Y, HitChainDistance.Z), true, true);
				break;
			case EChainDamagedType::E_Middle:
				m_pOwner->LaunchCharacter(FVector(HitChainDistance.X, m_pOwner->GetActionRotateValue() * HitChainDistance.Y, HitChainDistance.Z), true, true);
				break;
			case EChainDamagedType::E_Lower:
				m_pOwner->LaunchCharacter(FVector(HitChainDistance.X, m_pOwner->GetActionRotateValue() * HitChainDistance.Y, HitChainDistance.Z), true, true);
				break;
			default:
				UE_LOG(LogTemp,Error,TEXT("Others"));
				break;
			}

			m_pOwner->TFPlayerState->SetHitDetailState(ETFHitDetailState::E_Idle);
		}
	}
}

void UChainDamagedAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if(MeshComp !=nullptr && MeshComp->GetOwner() != nullptr && m_pOwner !=nullptr)
	{
		if(m_pOwner->TFPlayerState->GetHitDetailState() == ETFHitDetailState::E_Chain_Juggle_Finish && !bOnes)
		{
			bOnes = true;
			MeshComp->GetAnimInstance()->Montage_JumpToSection(FName("Finish"));
			m_pOwner->GetCharacterMovement()->GravityScale = m_pOwner->CombatComponent->GetDefaultGravityScale();
			UCapsuleComponent* Capsule = m_pOwner->GetCapsuleComponent();
			Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
			m_pOwner->ActionInputComponent->GetCurrentAction()->GetData()->NextActionNo = m_pOwner->GetSerialNo() * 10000 + 12;
			m_pOwner->ActiveDamegeBox(EStateofDamageBox::E_Invincible);
			m_pOwner->ActionInputComponent->SetDefaultLowAttack();
		}
		if(m_pOwner->TFPlayerState->GetCurrentHitState() != ETFHitState::E_Juggle && !bOnes)
		{
			bOnes = true;
			MeshComp->GetAnimInstance()->Montage_JumpToSection(FName("End"));
			m_pOwner->GetCharacterMovement()->GravityScale = m_pOwner->CombatComponent->GetDefaultGravityScale();
			UCapsuleComponent* Capsule = m_pOwner->GetCapsuleComponent();
			Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
			if(ChainDamagedType != EChainDamagedType::E_Middle)
			{
				m_pOwner->ActionInputComponent->GetCurrentAction()->GetData()->NextActionNo = m_pOwner->GetSerialNo() * 10000 + 13;
			}
			else
			{
				m_pOwner->ActionInputComponent->GetCurrentAction()->GetData()->NextActionNo = m_pOwner->GetSerialNo() * 10000 + 12;
			}
			m_pOwner->ActiveDamegeBox(EStateofDamageBox::E_Invincible);
			m_pOwner->ActionInputComponent->SetDefaultLowAttack();
		}
	}
}
