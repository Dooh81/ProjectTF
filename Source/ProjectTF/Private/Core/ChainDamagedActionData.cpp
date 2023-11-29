#include "Core/ChainDamagedActionData.h"

#include "Core/CombatComponent.h"
#include "Core/DataTableManager.h"
#include "GameFramework/CharacterMovementComponent.h"

#define START "Start"
#define LOOP "Loop"
#define END "End"

ChainDamagedActionData::ChainDamagedActionData(ActionDataTable* dataTable, APlayerCharacter* OwnerPlayer, APlayerCharacter* EnemyPlayer, FOnMontageBlendingOutStarted blendOutDelegate, float RotateValue) :
	ActionData(dataTable, OwnerPlayer, EnemyPlayer, blendOutDelegate, RotateValue)
{

}

void ChainDamagedActionData::Enter()
{
	ActionData::Enter();

	FString ani = m_pActionDataTable->AnimName.c_str();
	ani += "_Montage";

	FString CharacterName = m_pCharacterDataTable->Name.c_str();

	FString URL = "/Game/ArkWork/Characters/" + CharacterName + "/Animations/Montage/" + ani + "." + ani;

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

	ChainDamagedState = EChainDamagedState::E_Start;
	ChainDamagedType = EChainDamagedType::E_None;
	
	if(ani.Contains("Upper"))
	{
		ChainDamagedType = EChainDamagedType::E_Upper;
		ChainDamagedStartDistance = m_pOwner->CombatComponent->GetLauncherDistance(ChainDamagedType);
	}
	else if(ani.Contains("Middle"))
	{
		ChainDamagedType = EChainDamagedType::E_Middle;
		ChainDamagedStartDistance = m_pOwner->CombatComponent->GetLauncherDistance(ChainDamagedType);
	}
	else if(ani.Contains("Lower"))
	{
		ChainDamagedType = EChainDamagedType::E_Lower;
		ChainDamagedStartDistance = m_pOwner->CombatComponent->GetLauncherDistance(ChainDamagedType);
	}

	switch (ChainDamagedType)
	{
	case EChainDamagedType::E_None:
		UE_LOG(LogTemp,Error,TEXT("Others"));
		break;
	case  EChainDamagedType::E_Upper :
		m_pOwner->LaunchCharacter(FVector(ChainDamagedStartDistance.X, m_pOwner->GetActionRotateValue() * ChainDamagedStartDistance.Y, ChainDamagedStartDistance.Z), true, true);
		m_pOwner->GetCharacterMovement()->GravityScale = m_pOwner->CombatComponent->GetLauncherGravityScale(ChainDamagedType);
		break;
	case  EChainDamagedType::E_Middle :
		m_pOwner->LaunchCharacter(FVector(ChainDamagedStartDistance.X, m_pOwner->GetActionRotateValue() * ChainDamagedStartDistance.Y, ChainDamagedStartDistance.Z), true, true);
		m_pOwner->GetCharacterMovement()->GravityScale = m_pOwner->CombatComponent->GetDefaultGravityScale();
		break;
	case  EChainDamagedType::E_Lower :
		m_pOwner->LaunchCharacter(FVector(ChainDamagedStartDistance.X, m_pOwner->GetActionRotateValue() * ChainDamagedStartDistance.Y, ChainDamagedStartDistance.Z), true, true);
		m_pOwner->GetCharacterMovement()->GravityScale = m_pOwner->CombatComponent->GetLauncherGravityScale(ChainDamagedType);
		break;
	default:
		break;
	}
}

void ChainDamagedActionData::Exit()
{
	ActionData::Exit();
}

void ChainDamagedActionData::Tick(float DeltaTime)
{
	ActionData::Tick(DeltaTime);

	if(ChainDamagedState != EChainDamagedState::E_Loop)
	{
		return;
	}

	if(m_pOwner->GetCharacterMovement()->IsFalling() == false)
	{
		ChainDamagedState = EChainDamagedState::E_End;
	}
}

void ChainDamagedActionData::MontageBlendOut(UAnimMontage* animmontage, bool trigger)
{
	ActionData::MontageBlendOut(animmontage, trigger);

	if(ChainDamagedState != EChainDamagedState::E_Start)
	{
		return;
	}

	ChainDamagedState = EChainDamagedState::E_Loop;
}
