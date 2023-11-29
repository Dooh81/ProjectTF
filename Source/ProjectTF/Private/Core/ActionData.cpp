// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ActionData.h"

#include "Core/ActionInputComponent.h"
#include "Core/Commons.h"
#include "GameFramework/NewTokenFighterProtoGameModeBase.h"
#include "Core/DataTableManager.h"
#include "GameFramework/InGameGameMode.h"
#include "GameFramework/IngameGameState.h"

#define FORWARD_KEY "6"
#define BACKWARD_KEY "4"
#define CROUCH_KEY "2"
#define MIN 0.0f
#define MAX 1000.f


ActionData::CommandData::CommandData(std::string commnad, int interval)
{
	commnad = commnad.substr(0, commnad.length() - 1);
	commnad = commnad.substr(1, commnad.length());
	m_commandVec = split(commnad, '|');
	if (interval <= 0)
	{
		m_push = true;
		m_interval = (float)(-interval) / 1000.0f;
	}
	else
	{
		m_interval = (float)interval / 1000.0f;
	}

	GlobalSettingDataTable* globlaSetting = static_cast<GlobalSettingDataTable*>(DataTableManager::GetInstance()->
		GetDataTable("GlobalSetting", 1));
	if (globlaSetting->SerialNo == 1)
	{
		m_multiInterval = globlaSetting->MultiCommandInterval / 1000.0f;
	}
}

ActionData::ActionData(ActionDataTable* dataTable, APlayerCharacter* OwnerPlayer, APlayerCharacter* EnemyPlayer, FOnMontageBlendingOutStarted blendOutDelegate, float RotateValue)
{
	m_pActionDataTable = dataTable;
	m_commandVec.clear();
	
	std::list<string>::iterator CommandIter = dataTable->Command.begin();
	std::list<string>::iterator IntervalIter = dataTable->Interval.begin();
	while (CommandIter != dataTable->Command.end() && IntervalIter != dataTable->Command.end())
	{
		CommandData* commandData = new CommandData(*CommandIter, stoi(*IntervalIter));
		m_commandVec.push_back(commandData);
		++CommandIter;
		++IntervalIter;
	}
	
	fp_CurMoveUpdate = nullptr;
	fp_MoveUpdateArray[(int)ActionData::NONE_MOVE_TYPE] = nullptr;
	fp_MoveUpdateArray[(int)ActionData::FORWARD_MOVE_TYPE] = &ActionData::ForwardMoveUpdate;
	fp_MoveUpdateArray[(int)ActionData::RUN_MOVE_TYPE] = &ActionData::RunMoveUpdate;
	fp_MoveUpdateArray[(int)ActionData::BACK_MOVE_TYPE] = &ActionData::BackMoveUpdate;

	fp_CurMoveUpdate = fp_MoveUpdateArray[GetMoveType()];
	m_pOwner = OwnerPlayer;
	m_pEnemy = EnemyPlayer;
	m_fRotateValue = RotateValue;

	long serialNo = m_pOwner->GetSerialNo();

	m_pCharacterDataTable = (DataTable::CharacterDataTable*)DataTableManager::GetInstance()->GetDataTable("Character", serialNo);

	if (OwnerPlayer == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("OwnerPlayer nullptr"));
	}

	if (EnemyPlayer == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyPlayer nullptr"));
	}

	ContinueActionIntervalTime = -1.0f;
	if (m_pActionDataTable->PrevActionNo != 0)
	{
		GlobalSettingDataTable* globlaSetting = static_cast<GlobalSettingDataTable*>(DataTableManager::GetInstance()->
			GetDataTable("GlobalSetting", 1));

		ContinueActionIntervalTime = (float)globlaSetting->ContinueActionInterval / 1000.0f;
	}
	m_MontageBlendingOutStartedDelegate = blendOutDelegate;
	
}


ActionData::~ActionData()
{
	for (int i = 0; i < m_commandVec.size(); ++i)
	{
		CommandData* commandData = m_commandVec[i];
		delete commandData;
	}

	m_commandVec.clear();
}

void ActionData::Enter()
{
	int32 PlayerStateIndex = GetPlayerState();
	int32 AttackStateIndex = GetAttackState();

	ETFPlayerState PlayerState = ConvertIntToEnum<ETFPlayerState>(PlayerStateIndex);
	ETFAttackState AttackState = ConvertIntToEnum<ETFAttackState>(AttackStateIndex);

	m_pOwner->TFPlayerState->SetPlayerState(PlayerState);
	m_pOwner->TFPlayerState->SetAttackState(AttackState);
	
	FString ani = m_pActionDataTable->AnimName.c_str();

	ani += "_Montage";

	FString CharacterName = m_pCharacterDataTable->Name.c_str();

	FString URL = "/Game/ArtWork/Characters/" + CharacterName + "/Animations/Montage/" + ani + "." + ani;
	if (m_curAnim != nullptr)
	{
		m_curAnim->RateScale = 1.0f;
	}
	m_curAnim = LoadObject<UAnimMontage>(nullptr, *URL);

	m_enterTime = ANewTokenFighterProtoGameModeBase::GetCurrentTime();

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
	
}

void ActionData::Exit()
{
	m_exitTime = ANewTokenFighterProtoGameModeBase::GetCurrentTime();
	
	m_curAnim = nullptr;

	//Rotate
	if (m_pActionDataTable->RotateType == (int)ActionData::ENTER_ROTATE_TYPE)
	{
		RotateUpdate();
	}
}


void ActionData::CheckReset()
{
	m_current_index = 0;
	m_current_time = ANewTokenFighterProtoGameModeBase::GetCurrentTime(); // ������ ���� �ð� �������� �Լ��� ����

	if(m_pActionDataTable->SerialNo == m_pOwner->GetSerialNo() * 10000 + 12 || m_pActionDataTable->SerialNo == m_pOwner->GetSerialNo() * 10000 + 13)
	{
		m_pOwner->EnemyEndCombo();
	}
}

bool ActionData::CommandData::CommandCheck(float prevCommandtime, std::map<std::string, float>* pKeyDownMap, std::map<std::string, float>* pKeyUpMap)
{
	float minDownTime = FLT_MAX;
	float maxDownTime = -1.0f;
	int donwCnt = 0;

	for (int i = 0; i < m_commandVec.size(); ++i)
	{
		std::string key = m_commandVec[i];
		
		auto keyDownIter = pKeyDownMap->find(key);
		if (keyDownIter != pKeyDownMap->end())
		{
			auto keyUpIter = pKeyUpMap->find(keyDownIter->first);

			if(keyUpIter != pKeyUpMap->end())
			{
				if(m_commandVec.size() >= 1)
				{
					if(keyDownIter->second < keyUpIter->second)
					{
						return false;
					}
				}
			}
		}
		else
		{
			return false;
		}

		float downTime = keyDownIter->second;
		
		
		//�ȱ�� �ڷΰȱ�� ����
		// if (!(key == FORWARD_KEY || key == BACKWARD_KEY))
		// {
		// 	if (prevCommandtime >= downTime)
		// 	{
		// 		return false;
		// 	}
		// }

		if (minDownTime > downTime)
		{
			minDownTime = downTime;
		}

		if (maxDownTime < downTime)
		{
			maxDownTime = downTime;
		}

		donwCnt += 1;
	}

	if (maxDownTime - minDownTime > m_multiInterval ||
		donwCnt != m_commandVec.size())
	{
		return false;
	}


	if (m_push)
	{
		//CurrentTime() �𸮾� ���� �ð����� �����ؾ��Ѵ�.
		if (ANewTokenFighterProtoGameModeBase::GetCurrentTime() - minDownTime < m_interval)
		{
			return false;
		}
	}
	else
	{
		if (maxDownTime - prevCommandtime > m_interval)
		{
			return false;
		}
	}

	
	
	
	return true;
}

bool ActionData::ActionCheck(ActionData* prevAction, std::map<std::string, float>* pKeyDownMap, std::map<std::string, float>* pKeyUpMap)
{
	if ((m_pActionDataTable->PrevActionNo != 0 && prevAction == nullptr) ||
		(m_pActionDataTable->PrevActionNo != 0 && prevAction != nullptr && m_pActionDataTable->PrevActionNo != prevAction->GetData()->SerialNo))
	{
		return false;
	}

	for (; m_current_index < m_commandVec.size();)
	{
		if (m_commandVec[m_current_index]->CommandCheck(m_current_time, pKeyDownMap, pKeyUpMap) == false)
		{
			// Key�� Key ���� ���͹� ���� Ŭ��� �̾׼� Ŀ��带 ��� ���� ���Ѿ��Ѵ�. ó�� ���� �ٽ� �Է� ��Ų��.
			if (ANewTokenFighterProtoGameModeBase::GetCurrentTime() - m_current_time > m_commandVec[m_current_index]->GetInterval())
			{
				CheckReset();
			}
			return false;
		}

		if(m_commandVec[m_current_index]->GetInterval() > 0.0f && ANewTokenFighterProtoGameModeBase::GetCurrentTime() - m_current_time > m_commandVec[m_current_index]->GetInterval())
		{
			return false;
		}

		if (ContinueActionIntervalTime > 0.0f &&
			prevAction != nullptr)
		{
			if (ANewTokenFighterProtoGameModeBase::GetCurrentTime() - prevAction->EnterTime() > ContinueActionIntervalTime)
			{
				return false;
			}
		}

		++m_current_index;
		m_current_time = ANewTokenFighterProtoGameModeBase::GetCurrentTime(); // �𸮾� Time���� ���� �ؾ��Ѵ�.
	}

	return true;
}

bool ActionData::EndCheck(std::map<std::string, float>* pKeyDownMap, std::map<std::string, float>* pKeyUpMap)
{
	// �ִϸ��̼� ���� End�� �̷�� ����� ���� ���� ���������� üũ�� ���� �ʴ´�.
	

	if (m_pActionDataTable->EndType == END_TYPE::ANI_END)
	{
		return false;
	}
	
	if (m_pActionDataTable->EndType == END_TYPE::TIME_END)
	{
		if (ANewTokenFighterProtoGameModeBase::GetCurrentTime() - m_enterTime >= m_pActionDataTable->NextActionInterval / 1000.0f)
		{
			return true;
		}
	}
	else if (m_pActionDataTable->EndType == END_TYPE::COMMAND_END)
	{
		for (int i = 0; i < m_commandVec.size(); ++i)
		{
			std::vector<std::string>* pCommandVec = m_commandVec[i]->GetCommandVec();
			for (int k = 0; k < pCommandVec->size(); ++k)
			{
				auto keyDowniter = pKeyDownMap->find((*pCommandVec)[k]);
				
				if(keyDowniter != pKeyDownMap->end())
				{
					auto keyUpIter = pKeyUpMap->find(keyDowniter->first);
		
					if(keyUpIter != pKeyUpMap->end())
					{
						if(m_commandVec.size() == 1)
						{
							if(keyDowniter->second < keyUpIter->second)
							{
								return true;
							}
						}
					}
				}
			}
		}
	}

	return false;
}

unsigned long ActionData::CancelActionCheck(std::map<std::string, float>* pKeyDownMap, std::map<std::string, float>* pKeyUpMap)
{
	for (int i = 0; i < m_commandVec.size(); ++i)
	{
		std::vector<std::string>* pCommandVec = m_commandVec[i]->GetCommandVec();
		for (int k = 0; k < pCommandVec->size(); ++k)
		{
			auto keyDowniter = pKeyDownMap->find((*pCommandVec)[k]);
				
			if(keyDowniter != pKeyDownMap->end())
			{
				auto keyUpIter = pKeyUpMap->find(keyDowniter->first);
		
				if(keyUpIter != pKeyUpMap->end())
				{
					if(m_commandVec.size() == 1)
					{
						if(keyDowniter->second > m_enterTime && keyDowniter->second > keyUpIter->second)
						{
							keyUpIter->second = m_enterTime;

							auto CrouchDown = pKeyDownMap->find(CROUCH_KEY);
							if(CrouchDown != pKeyDownMap->end())
							{
								auto CrouchUp = pKeyUpMap->find(CROUCH_KEY);
								if(CrouchUp != pKeyUpMap->end())
								{
									if(CrouchDown->second > CrouchUp->second)
									{
										//UE_LOG(LogTemp,Warning,TEXT("Crouch"));
										return 23;
									}
									else
									{
										//UE_LOG(LogTemp,Warning,TEXT("Stand"));
										return 83;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	
	return 0;
}

ActionData::END_TYPE ActionData::GetEndType()
{
	return (ActionData::END_TYPE)m_pActionDataTable->EndType;
}

ActionData::MOVE_TYPE ActionData::GetMoveType()
{
	return (ActionData::MOVE_TYPE)m_pActionDataTable->MoveType;
}

ActionData::ROTATE_TYPE ActionData::GetRotateType()
{
	return (ActionData::ROTATE_TYPE)m_pActionDataTable->RotateType;
}

ActionData::PLAYER_STATE ActionData::GetPlayerState()
{
	return (ActionData::PLAYER_STATE)m_pActionDataTable->PlayerState;
}

ActionData::ATTACK_STATE ActionData::GetAttackState()
{
	return (ActionData::ATTACK_STATE)m_pActionDataTable->AttackState;
}

bool ActionData::IsCatchType()
{
	return (ActionData::CATCH_TYPE)m_pActionDataTable->CatchType == ActionData::NONE_CATCH_TYPE ? false : true;
}

bool ActionData::IsBlockType()
{
	return (ActionData::BLOCK_USE_TYPE)m_pActionDataTable->BlockUseType == ActionData::NOT_USE_BLOCK_TYPE ? false : true;
}

void ActionData::Tick(float DeltaTime)
{
	//Movement
	if (fp_CurMoveUpdate != nullptr)
	{
		AInGameGameMode* IngameGameMode = Cast<AInGameGameMode>(UGameplayStatics::GetGameMode(m_pOwner->GetWorld()));
		if (IngameGameMode)
		{
			if (IngameGameMode->IngameState->GetIngameState() == EInGameState::E_PLAYING)
			{
				(this->*fp_CurMoveUpdate)(DeltaTime);
			}
		}
	}

	//Rotate
	if (m_pActionDataTable->RotateType == (int)ActionData::TICK_ROTATE_TYPE)
	{
		RotateUpdate();
	}
}

void ActionData::ForwardMoveUpdate(float deltaTime)
{
	FVector Direction = -m_pOwner->GetActorRightVector();
	
	Direction.Z = 0.0f;
	Direction.Normalize();

	float ForwardWalk = m_pCharacterDataTable->Foward_Walk / MAX;
	float Clamp = FMath::Clamp(ForwardWalk, MIN, MAX);
	m_pOwner->MoveValue = Clamp;
	m_pOwner->AddMovementInput(Direction, m_pOwner->GetActionRotateValue() * Clamp);
}

void ActionData::RunMoveUpdate(float deltaTime)
{
	FVector Direction = -m_pOwner->GetActorRightVector();
	Direction.Z = 0.0f;
	Direction.Normalize();

	float ForwardDash = m_pCharacterDataTable->Foward_Dash / MAX;
	float Clamp = FMath::Clamp(ForwardDash, MIN, MAX);
	m_pOwner->MoveValue = Clamp;
	m_pOwner->AddMovementInput(Direction, m_pOwner->GetActionRotateValue() * Clamp);
}

void ActionData::BackMoveUpdate(float deltaTime)
{
	FVector Direction = -m_pOwner->GetActorRightVector();
	
	Direction.Z = 0.0f;
	Direction.Normalize();
	float BackwardWalk = m_pCharacterDataTable->Back_Walk / MAX;
	float Clamp = FMath::Clamp(BackwardWalk, MIN, MAX);
	m_pOwner->MoveValue = Clamp;
	m_pOwner->AddMovementInput(Direction, -(m_pOwner->GetActionRotateValue() * Clamp));
}


void ActionData::RotateUpdate()
{
	FVector currentPos = m_pOwner->GetActorLocation();
	FVector enemyPos = m_pEnemy->GetActorLocation();

	FVector dir = enemyPos - currentPos;
	dir.Z = 0.0f;
	dir.Normalize();

	FVector forward = m_pOwner->GetActionRotateValue() * -m_pOwner->GetActorRightVector();
	forward.Z = 0.0f;
	forward.Normalize();
	
	double dot = FVector::DotProduct(dir, forward);

	if(dot < 0)
	{
		m_pOwner->ActionInputComponent->SetRotateValue();

		m_pOwner->bMirror = !m_pOwner->bMirror;

		if(m_pOwner->ActionInputComponent->GetCurrentAction()->GetData()->SerialNo == m_pOwner->GetSerialNo()* 10000 + 1)
		{
			m_pOwner->ActionInputComponent->ChangeAction(m_pOwner->GetSerialNo()* 10000 + 97);
		}
		else if(m_pOwner->ActionInputComponent->GetCurrentAction()->GetData()->SerialNo == m_pOwner->GetSerialNo()* 10000 + 2)
		{
			m_pOwner->ActionInputComponent->ChangeAction(m_pOwner->GetSerialNo()* 10000 + 98);
		}
		
		m_pOwner->SkillCamera->SetRelativeLocation(FVector(m_pOwner->SkillCamera->GetRelativeLocation().X, -m_pOwner->SkillCamera->GetRelativeLocation().Y, m_pOwner->SkillCamera->GetRelativeLocation().Z));
		m_pOwner->SkillCamera->SetRelativeRotation(FRotator(m_pOwner->SkillCamera->GetRelativeRotation().Pitch, -m_pOwner->SkillCamera->GetRelativeRotation().Yaw,-m_pOwner->SkillCamera->GetRelativeRotation().Roll));
		
		if(!m_pOwner->ActionInputComponent->GetGuardValue())
		{
			m_pOwner->ActionInputComponent->ChangeRotateInput();
		}

		
		 FTimerHandle TimerHandle;
		 m_pOwner->GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
		 	{
		 		m_pOwner->fMirrorBlendTime = 0.5f;
		 	}), .5f, false);
	}
}

void ActionData::MontageBlendOut(class UAnimMontage* animmontage, bool trigger)
{
	//UE_LOG(LogTemp, Warning, TEXT("MontageBlendOut Anim Name : %s"), *animmontage->GetName());
}
