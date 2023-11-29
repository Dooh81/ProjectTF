// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ActionInputComponent.h"

#include "Core/ChainDamagedActionData.h"
#include "Core/CombatComponent.h"
#include "GameFramework/NewTokenFighterProtoGameModeBase.h"
#include "Core/DataTableManager.h"
#include "Core/JumpActionData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InGameGameMode.h"
#include "GameFramework/IngameGameState.h"
#include <algorithm>


#define JUMP "Jump"; // 점프 관련 예약어
#define CHAINDAMAGED "ChainDamaged";
#define FORWARD_KEY "6" //방향키 6 예약어
#define BACKWARD_KEY "4" //방향키 4 예약어
#define CROUCH_KEY "2"
#define JUMP_KEY "J"
#define WALK_BACK 4
#define CROUCH 2
#define GUARD_ST 8
#define GUARD_CR 9
#define STAND_STATE 1
#define DODGE_FRONT_KEY 7
#define ATK_ST_L 83

#define QUEUE_CHECK_TIME 1.0f //커맨드 큐 체크 시간

#define ACTION_CHECK_TIME 0.02f //액션 체크타임 0.04f

// Sets default values for this component's properties
UActionInputComponent::UActionInputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	m_bActionCheck = true;


	m_keyArray[(int)EInputKey::KEY_1] = "1";
	m_keyArray[(int)EInputKey::KEY_2] = "2";
	m_keyArray[(int)EInputKey::KEY_3] = "3";
	m_keyArray[(int)EInputKey::KEY_4] = "4";
	m_keyArray[(int)EInputKey::KEY_5] = "5";
	m_keyArray[(int)EInputKey::KEY_6] = "6";
	m_keyArray[(int)EInputKey::KEY_7] = "7";
	m_keyArray[(int)EInputKey::KEY_8] = "8";
	m_keyArray[(int)EInputKey::KEY_9] = "9";
	m_keyArray[(int)EInputKey::KEY_Low] = "L";
	m_keyArray[(int)EInputKey::KEY_High] = "H";
	m_keyArray[(int)EInputKey::KEY_Jump] = "J";
	m_keyArray[(int)EInputKey::KEY_Parry] = "P";
	m_keyArray[(int)EInputKey::KEY_Throw] = "T";
	m_keyArray[(int)EInputKey::KEY_Special] = "S";

	m_pOwner = Cast<APlayerCharacter>(GetOwner());

	ActionCheckTime = -1.0f;
}


// Called when the game starts
void UActionInputComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UActionInputComponent::BeginDestroy()
{
	Super::BeginDestroy();

	for (std::map<unsigned long, ActionData*>::iterator iter = m_actionMap.begin(); iter != m_actionMap.end(); ++iter)
	{
		ActionData* actionData = iter->second;
		delete actionData;
	}

	m_actionMap.clear();
	m_chainActionVec.clear();
	m_keydownmap.clear();
	m_keyupmap.clear();
}


void UActionInputComponent::InputDownAction(std::string key)
{
	if (m_keydownmap.find(key) != m_keydownmap.end())
	{
		m_keydownmap.find(key)->second = ANewTokenFighterProtoGameModeBase::GetCurrentTime();
	}
	else
	{
		m_keydownmap.insert(std::make_pair(key, ANewTokenFighterProtoGameModeBase::GetCurrentTime()));
	}

	// if(!GetWorld()->GetTimerManager().IsTimerActive(CommandQueueTimerHandle))
	// {
	// 	m_commandQueue = queue<std::string>();
	// 	GetWorld()->GetTimerManager().SetTimer(CommandQueueTimerHandle, this, &UActionInputComponent::CommandDebug, QUEUE_CHECK_TIME, false);
	// }
	//
	// m_commandQueue.push(key);
}

void UActionInputComponent::InputDownAction(EInputKey key)
{
	AInGameGameMode* IngameGameMode = Cast<AInGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IngameGameMode)
	{
		if(IngameGameMode->IngameState->GetIngameState() == EInGameState::E_PLAYING)
		{
			InputDownAction(m_keyArray[(int)key]);
		}
		else if(IngameGameMode->IngameState->GetIngameState() == EInGameState::E_INTRO)
		{
			if(key == EInputKey::KEY_Jump)
			{
				IngameGameMode->CheckGameState();
			}
		}
	}
}

void UActionInputComponent::InputUpAction(std::string key)
{
	FString DebugKey = key.c_str();

	if (m_keyupmap.find(key) != m_keyupmap.end())
	{
		m_keyupmap.find(key)->second = ANewTokenFighterProtoGameModeBase::GetCurrentTime();
	}
	else
	{
		m_keyupmap.insert(std::make_pair(key, ANewTokenFighterProtoGameModeBase::GetCurrentTime()));
	}
}

void UActionInputComponent::InputUpAction(EInputKey key)
{
	InputUpAction(m_keyArray[(int)key]);
}

void UActionInputComponent::ChangeAction(unsigned long serialNo)
{
	auto iter = m_actionMap.find(serialNo);
	if (iter == m_actionMap.end())
	{
		return;
	}

	m_chainActionVec.clear();
	m_pCurAction = iter->second;

	//디폴트 액션이면 prev 액션에 저장하지 않는다.
	if (m_pCurAction != m_pDefaultAction)
	{
		m_pPrevAction = m_pCurAction;
	}

	if (m_pCurAction != nullptr)
	{
		m_pCurAction->Exit();
	}

	m_pCurAction->Enter();

	Chain_Atk_List = m_pCurAction->GetData()->Chain_Atk_List;

	if (serialNo == m_pOwner->GetSerialNo() * 10000 + STAND_STATE && m_pOwner->TFPlayerState->GetAttackDetailState() ==
		ETFAttackDetailState::E_Launcher)
	{
		ActionData* AppendActionData;
		auto appendIter = m_actionMap.find(m_pAppendActionNo);
		if (appendIter != m_actionMap.end())
		{
			AppendActionData = appendIter->second;

			auto begin = Chain_Atk_List.begin();
			for (; begin != Chain_Atk_List.end(); ++begin)
			{
				iter = m_actionMap.find(*begin);

				unsigned long DefaultAttackNo = m_pOwner->GetSerialNo() * 10000 + ATK_ST_L;

				if (iter != m_actionMap.end())
				{
					if (DefaultAttackNo == iter->first)
					{
						m_chainActionVec.push_back(AppendActionData);
					}
					else
					{
						m_chainActionVec.push_back(iter->second);
					}
				}
			}
		}
	}
	else
	{
		auto begin = Chain_Atk_List.begin();
		for (; begin != Chain_Atk_List.end(); ++begin)
		{
			iter = m_actionMap.find(*begin);

			if (iter != m_actionMap.end())
			{
				m_chainActionVec.push_back(iter->second);
			}
		}
	}

	AnimName = m_pCurAction->GetData()->AnimName.c_str();

	m_pCurAction->CheckReset();
	m_pOwner->AddSP(m_pCurAction);

	//string command = m_pCurAction->GetData()->Command[0];

	// 애니매이션을 플레이한다.
	FColor Color = FColor::Yellow;

	if(AnimName == "_Stand")
	{
		Color = FColor::Red;
	}
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, Color, FString::Printf(TEXT("ActionName : %s"), *AnimName));
}

ActionData* UActionInputComponent::GetActionDataByNum(unsigned long serialNo)
{
	auto iter = m_actionMap.find(serialNo);
	if (iter == m_actionMap.end())
	{
		return nullptr;
	}
	return iter->second;
}

float UActionInputComponent::GetActionMontageTime(unsigned long serialNo)
{
	unsigned long CallSerialNo = m_pOwner->GetSerialNo() * 10000 + serialNo;
	const ActionDataTable* Data = GetActionDataByNum(CallSerialNo)->GetData();

	FString ani = Data->AnimName.c_str();
	ani += "_Montage";
	FString CharacterName = m_pCharacterDataTable->Name.c_str();
	FString URL = "/Game/ArtWork/Characters/" + CharacterName + "/Animations/Montage/" + ani + "." + ani;

	float returnValue = 0.0f;
	const UAnimMontage* AnimMontage = LoadObject<UAnimMontage>(nullptr, *URL);

	if (AnimMontage != nullptr)
	{
		returnValue = AnimMontage->GetPlayLength();
		AnimMontage = nullptr;
	}

	return returnValue;
}

bool UActionInputComponent::GameStateEnd()
{
	bool result = false;
	AInGameGameMode* IngameGameMode = Cast<AInGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IngameGameMode)
	{
		if(IngameGameMode->IngameState->GetIngameState() == EInGameState::E_END)
		{
			result = true;
		}

		result = false;
	}

	return result;
}

// Called every frame
void UActionInputComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FrameViewer();

	if (m_pCurAction == nullptr) return;
	

	// 액션 검사를 수행한다.
	// 액션이 바뀌었으면 return 시킨다.
	if (ActionCheck() == true)
	{
		return;
	}

	if (m_pCurAction != nullptr)
	{
		m_pCurAction->Tick(DeltaTime);
	}

	if (m_pCurAction->EndCheck(&m_keydownmap, &m_keyupmap) == true)
	{
		//next action no가 1번(stand)가 아닌경우 플레이어상태를 다음 애니메이션으로 처리해야됨.
		// bool IsNextAnimationDefault = m_pCurAction->GetData()->NextActionNo == m_pOwner->CurrentSerialNo * 10000 +1 ? true:false;
		// if(IsNextAnimationDefault)
		// {
		// 	if(IsStand())
		// 	{
		// 		ChangeAction(m_pCurAction->GetData()->NextActionNo);
		// 	}
		// }
		// else
		// {
		// 	ChangeAction(m_pCurAction->GetData()->NextActionNo);
		// }

		ChangeAction(m_pCurAction->GetData()->NextActionNo);

		FTimerHandle TimerHandle;
		m_pOwner->GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			m_bGuardCheck = false;
		}), 1.0f, false);

		return;
	}
}

void UActionInputComponent::ChangeActionAnimation()
{
	if (m_pCurAction->GetEndType() != ActionData::ANI_END)
	{
		return;
	}

	// bool IsNextAnimationDefault = m_pCurAction->GetData()->NextActionNo == m_pOwner->CurrentSerialNo * 10000 +1 ? true:false;
	// if(IsNextAnimationDefault)
	// {
	// 	if(IsStand())
	// 	{
	// 		ChangeAction(m_pCurAction->GetData()->NextActionNo);
	// 	}
	// }
	// else
	// {
	// 	ChangeAction(m_pCurAction->GetData()->NextActionNo);
	// }

	ChangeAction(m_pCurAction->GetData()->NextActionNo);
}

bool UActionInputComponent::GetGuardValue()
{
	return m_bGuardCheck;
}

bool UActionInputComponent::GuardCheck(long AttackPosition)
{
	long CurrentAnimNo = m_pOwner->GetSerialNo() * 10000;

	std::string UpperGuardKey = BACKWARD_KEY;
	std::string LowerGuardKey = CROUCH_KEY;

	auto keyUpperDowniter = m_keydownmap.find(UpperGuardKey);
	auto keyUpperUpIter = m_keyupmap.find(UpperGuardKey);
	auto keyLowerDowniter = m_keydownmap.find(LowerGuardKey);
	auto keyLowerUpiter = m_keyupmap.find(LowerGuardKey);

	bool bUpperGuard = false;
	bool bLowerGuard = false;
	//AnimName

	//FString AnimNaming = m_pOwner->GetMesh()->GetAnimInstance()->GetActiveMontageInstance()->Montage->GetName();
	// if(AnimName.Contains("Damaged"))
	// {
	// 	return false;
	// }

	if (m_keydownmap.find(UpperGuardKey) != m_keydownmap.end())
	{
		if (keyUpperDowniter->second > keyUpperUpIter->second)
		{
			bUpperGuard = true;
		}
	}

	if (m_keydownmap.find(LowerGuardKey) != m_keydownmap.end() && m_keydownmap.find(UpperGuardKey) != m_keydownmap.
		end())
	{
		if (keyLowerDowniter->second > keyLowerUpiter->second && keyUpperDowniter->second > keyUpperUpIter->second)
		{
			bLowerGuard = true;
		}
	}

	if (AttackPosition != 0 && m_pOwner->HitterState == EHitterState::E_Idle && !m_pOwner->GetCharacterMovement()->IsFalling())
	{
		if (AttackPosition == 1)
		{
			//상,하단 중 하나만 성립시 가드성공
			if (bUpperGuard && !bLowerGuard)
			{
				m_bGuardCheck = true;
				ChangeAction(CurrentAnimNo + GUARD_ST);
				return true;
			}

			if (bLowerGuard)
			{
				m_bGuardCheck = true;
				ChangeAction(CurrentAnimNo + GUARD_CR);
				return true;
			}
		}

		if (AttackPosition == 2)
		{
			//상단 가드 성립시 가드성공
			if (bUpperGuard && !bLowerGuard)
			{
				m_bGuardCheck = true;
				ChangeAction(CurrentAnimNo + GUARD_ST);
				return true;
			}
		}

		if (AttackPosition == 3)
		{
			//하단 가드 성립시 가드성공
			if (bLowerGuard)
			{
				m_bGuardCheck = true;
				ChangeAction(CurrentAnimNo + GUARD_CR);
				return true;
			}
		}
	}

	m_bGuardCheck = false;
	return false;
}

void UActionInputComponent::InitData(APlayerCharacter* PlayerCharacter, APlayerCharacter* EnemyCharacter,
                                     float RotateValue)
{
	m_pOwner = PlayerCharacter;
	m_pEnemy = EnemyCharacter;
	m_fRotateValue = RotateValue;

	long serialNo = m_pOwner->GetSerialNo();

	m_pCharacterDataTable = (DataTable::CharacterDataTable*)DataTableManager::GetInstance()->GetDataTable(
		"Character", serialNo);

	float fJump = m_pOwner->TestJumpHeight;
	float fForwardJump = -m_pOwner->TestJumpForward;
	float fBackJump = m_pOwner->TestJumpBackward;

	//float fJump = (float)m_pCharacterDataTable->Jump;
	//float fForwardJump = -(float)m_pCharacterDataTable->Forward_Jump;
	//float fBackJump =(float)m_pCharacterDataTable->Back_Jump;

	m_JumpVelocity[(int)EJumpState::E_Vertical].Set(0.0f, 0.0f, fJump);
	m_JumpVelocity[(int)EJumpState::E_Forward].Set(0.0f, fForwardJump, fJump);
	m_JumpVelocity[(int)EJumpState::E_Back].Set(0.0f, fBackJump, fJump);


	FOnMontageBlendingOutStarted blendOutDelegate;
	blendOutDelegate.BindUObject(this, &UActionInputComponent::MontageBlendOut);

	std::string jump = JUMP;
	std::string ChainDamaged = CHAINDAMAGED;
	std::vector<DataTable::DataTableBase*>* pActionDataTableList = DataTableManager::GetInstance()->GetActionDataTable(serialNo);
	for (std::vector<DataTable::DataTableBase*>::iterator iter = pActionDataTableList->begin(); iter !=pActionDataTableList->end(); ++iter)
	{
		ActionDataTable* pActionDataTable = static_cast<ActionDataTable*>(*iter);
		
		ActionData* pActionData = nullptr;
		if (pActionDataTable->AnimName.find(jump) != string::npos)
		{
			pActionData = new JumpActionData(pActionDataTable, m_pOwner, m_pEnemy, blendOutDelegate, m_fRotateValue);
		}
		else if (pActionDataTable->AnimName.find(ChainDamaged) != string::npos)
		{
			pActionData = new ChainDamagedActionData(pActionDataTable, m_pOwner, m_pEnemy, blendOutDelegate,
			                                         m_fRotateValue);
		}
		else
		{
			pActionData = new ActionData(pActionDataTable, m_pOwner, m_pEnemy, blendOutDelegate, m_fRotateValue);
		}

		if(pActionDataTable->SerialNo != 0 && pActionData != nullptr)
		{
			m_actionMap.insert({pActionDataTable->SerialNo, pActionData});
		}
	}

	
	//기존 정렬된 맵을 사용하지 않고 우선순위로 정렬된 unorderedmap을 사용한다. 속도도 빠름
	// std::vector<pair<unsigned long, ActionData*>> sortVector;
	// for (std::map<unsigned long, ActionData*>::iterator it = m_actionMap.begin(); it != m_actionMap.end(); ++it)
	// {
	// 	sortVector.push_back(std::make_pair(it->first, it->second));
	// }
	//
	// sort(sortVector.begin(), sortVector.end(),SortByValue);
	//
	// for(std::vector<pair<unsigned long, ActionData*>>::iterator it = sortVector.begin(); it != sortVector.end(); ++it)
	// {
	// 	m_sortMap.insert(make_pair(it->first, it->second));
	// }
	//
	// sortVector.clear();
	//
	// for (std::unordered_map<unsigned long, ActionData*>::iterator it = m_sortMap.begin(); it != m_sortMap.end(); ++it)
	// {
	// 	UE_LOG(LogTemp,Warning,TEXT("sortMap Index: %lu"), it->first);
	// }

	std::map<unsigned long, ActionData*>::iterator iter = m_actionMap.find(m_pCharacterDataTable->DefaultActionNo);
	if (iter == m_actionMap.end())
	{
		return;
	}

	m_pDefaultAction = iter->second;

	ChangeAction(m_pCharacterDataTable->DefaultActionNo);
}

bool UActionInputComponent::ActionCheck()
{
	if (ActionCheckTime != -1 && ANewTokenFighterProtoGameModeBase::GetCurrentTime() - ActionCheckTime <
		ACTION_CHECK_TIME)
	{
		return false;
	}

	ActionCheckTime = ANewTokenFighterProtoGameModeBase::GetCurrentTime();

	if (m_bActionCheck == false)
	{
		return false;
	}

	
	auto begin = m_chainActionVec.begin();
	for (; begin != m_chainActionVec.end(); ++begin)
	{
		ActionData* actionData = (*begin);
		if (actionData->ActionCheck(m_pPrevAction, &m_keydownmap, &m_keyupmap) == true)
		{
			if (actionData->GetData()->SerialNo == m_pOwner->GetSerialNo() * 10000 + DODGE_FRONT_KEY)
			{
				if (GetGuardValue())
				{
					ChangeAction(actionData->GetData()->SerialNo);
					UE_LOG(LogTemp, Warning, TEXT("ActionCheck() ChangeAction %llu"), actionData->GetData()->SerialNo);
					m_bActionCheck = false;
					return true;
				}
			}
			else
			{
				if(m_pOwner->TFPlayerState->GetAttackDetailState() != ETFAttackDetailState::E_Launcher && m_pCurAction->GetData()->Juggle > 0)
				{
					ChangeAction(m_pOwner->GetSerialNo() * 10000 + ATK_ST_L);
					UE_LOG(LogTemp, Warning, TEXT("ActionCheck() ChangeAction %llu"), actionData->GetData()->SerialNo);
					m_bActionCheck = false;
					return true;
				}
				else
				{
					ChangeAction(actionData->GetData()->SerialNo);
					UE_LOG(LogTemp, Warning, TEXT("ActionCheck() ChangeAction %llu"), actionData->GetData()->SerialNo);
					m_bActionCheck = false;
					return true;
				}
				
			}
		}
	}

	return false;
}

void UActionInputComponent::ChangeRotateInput()
{
	// 좌우를 바꿔 줘야한다.
	auto KeyForwardIter = m_keydownmap.find(FORWARD_KEY);
	float ForwardDownTime = -1.0f;
	if (KeyForwardIter != m_keydownmap.end())
	{
		ForwardDownTime = KeyForwardIter->second;
		m_keydownmap.erase(KeyForwardIter);
	}

	auto KeyBackIter = m_keydownmap.find(BACKWARD_KEY);
	float BackDownTime = -1.0f;
	if (KeyBackIter != m_keydownmap.end())
	{
		BackDownTime = KeyBackIter->second;
		m_keydownmap.erase(KeyBackIter);
	}

	if (ForwardDownTime > 0.0f)
	{
		m_keydownmap.insert(std::make_pair(BACKWARD_KEY, ForwardDownTime));
	}

	if (BackDownTime > 0.0f)
	{
		m_keydownmap.insert(std::make_pair(FORWARD_KEY, BackDownTime));
	}

	auto KeyForwardUpIter = m_keyupmap.find(FORWARD_KEY);
	float ForwardUpTime = -1.0f;
	if (KeyForwardUpIter != m_keyupmap.end())
	{
		ForwardUpTime = KeyForwardUpIter->second;
		m_keyupmap.erase(KeyForwardUpIter);
	}

	auto KeyBackUpIter = m_keyupmap.find(BACKWARD_KEY);
	float BackUpTime = -1.0f;
	if (KeyBackUpIter != m_keyupmap.end())
	{
		BackUpTime = KeyBackUpIter->second;
		m_keyupmap.erase(KeyBackUpIter);
	}

	if (ForwardUpTime > 0.0f)
	{
		m_keyupmap.insert(std::make_pair(BACKWARD_KEY, ForwardUpTime));
	}

	if (BackUpTime > 0.0f)
	{
		m_keyupmap.insert(std::make_pair(FORWARD_KEY, BackUpTime));
	}
}

void UActionInputComponent::InitailizeInput()
{
	for (const auto& key : m_keyArray)
	{
		m_keydownmap.insert(std::make_pair(key, 0.f));
		m_keyupmap.insert(std::make_pair(key, 0.001f));
	}
}

void UActionInputComponent::CommandDebug()
{
	FString AppendString;

	while (!m_commandQueue.empty())
	{
		AppendString += m_commandQueue.front().c_str();
		m_commandQueue.pop();
	}

	UE_LOG(LogTemp, Warning, TEXT("Command : %s"), *AppendString);
}

void UActionInputComponent::HitOrGuardStop(bool GuardState)
{
	//hit 및 가드 피격이 발생했을때발생했을때 경직되어야 함 (공격자와 피격자 둘다 경직)
	float HitInterval = m_pEnemy->ActionInputComponent->GetCurrentAction()->GetData()->HitInterval / 1000.f;
	if (HitInterval > 0)
	{
		m_pOwner->ActionInputComponent->StiffStop(m_pEnemy, HitInterval, false);
		m_pEnemy->ActionInputComponent->StiffStop(m_pOwner, HitInterval, true);
	}
}

void UActionInputComponent::StiffStop(APlayerCharacter* AttackCaster, float StunTime, bool isAttacker)
{
	m_pOwner->CustomTimeDilation = 0.001f;
	FTimerHandle TimerHandle;
	float BlockStun = AttackCaster->ActionInputComponent->GetCurrentAction()->GetData()->BlockStun;
	float HitStun = AttackCaster->ActionInputComponent->GetCurrentAction()->GetData()->HitStun;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=]()
	{
		m_pOwner->CustomTimeDilation = 1.0f;
		if (!isAttacker)
		{
			float MoveBackTime;
			if (m_pOwner->ActionInputComponent->GetGuardValue())
			{
				MoveBackTime = BlockStun;
			}
			else
			{
				MoveBackTime = HitStun;
			}

			StiffStopActionSender(MoveBackTime);
		}
		else
		{
			SetAttackState(EAttackerState::E_Idle);
		}
	}), StunTime, false);
}

void UActionInputComponent::StiffStopActionSender(float MoveBackTime)
{
	float Distance = m_pOwner->ActionInputComponent->GetRotateValue() * 0.1f;

	FVector Displacement = FVector(0.0f, Distance, 0.0f);

	StunBackMoveSendParameterDelegate.BindUObject(this, &UActionInputComponent::StunBackMoveDistance, Displacement);

	if (GEngine)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			float Rate = MoveBackTime / 1000.0f;
			float RateCheck = Rate / MoveBackTime;
			World->GetTimerManager().SetTimer(StunBackMoveHandle, StunBackMoveSendParameterDelegate, RateCheck, true);

			World->GetTimerManager().SetTimer(StunBackMoveEndHandle, FTimerDelegate::CreateLambda([this]()
			{
				GetWorld()->GetTimerManager().ClearTimer(StunBackMoveHandle);
			}), Rate, false);
		}
	}
}

void UActionInputComponent::StunBackMoveDistance(FVector TicMoveLocation)
{
	FVector MoveLocation = m_pOwner->GetActorLocation() + TicMoveLocation;
	m_pOwner->SetActorLocation(MoveLocation, true);
}

void UActionInputComponent::SetAttackState(EAttackerState AtkState)
{
	m_pOwner->AttackerState = AtkState;
}

void UActionInputComponent::SetLaucherState(ELauncherState LauncherState)
{
	m_pOwner->LauncherState = LauncherState;
}

void UActionInputComponent::SetLauncherSerialNo(const ulong_t& AppendAction)
{
	//chainActionIgnoreList를 만들어서 default 약어택, 상중하 콤보중 해당 인덱스 제외하고 ignore
	m_pAppendActionNo = AppendAction;
}

void UActionInputComponent::ClearAppendAction()
{
	m_pAppendActionNo = 0;
}

void UActionInputComponent::SetDefaultLowAttack()
{
	m_pOwner->TFPlayerState->SetCurrentHitState(ETFHitState::E_Idle);
	EndCombo();
}

void UActionInputComponent::SetDefaultLowAttackEnemy()
{
	m_chainActionVec.clear();
	
	auto iter = m_actionMap.find(m_pOwner->GetSerialNo() * 10000 + ATK_ST_L);
	auto begin = Chain_Atk_List.begin();
	for (; begin != Chain_Atk_List.end(); ++begin)
	{
		iter = m_actionMap.find(*begin);
		if (iter != m_actionMap.end())
		{
			m_chainActionVec.push_back(iter->second);
		}
	}
}

void UActionInputComponent::EndCombo()
{
	m_pEnemy->TFPlayerState->SetAttackDetailState(ETFAttackDetailState::E_Idle);
	// m_pEnemy->ActionInputComponent->SetAttackState(EAttackerState::E_Idle);
	// m_pEnemy->ActionInputComponent->SetLaucherState(ELauncherState::E_Idle);
	m_pEnemy->ActionInputComponent->ClearAppendAction();
	m_pEnemy->FinishState = EUseFinishState::E_None;
	m_pEnemy->TFPlayerState->ResetJuggleCount();
	m_pEnemy->TFPlayerState->ResetComboCount();
	m_pEnemy->ActionInputComponent->SetDefaultLowAttackEnemy();
}

bool UActionInputComponent::IsStand()
{
	bool isStand = true;

	for(std::map < std::string, float >::iterator it = m_keydownmap.begin(); it != m_keydownmap.end(); ++it)
	{
		auto downiter = it;
		auto upiter = m_keyupmap.find(it->first);

		if(upiter->second < downiter->second)
		{
			isStand = false;
		}
	}
	
	return isStand;
}

unsigned long UActionInputComponent::CancelActionCheck()
{
	return GetCurrentAction()->CancelActionCheck(&m_keydownmap, &m_keyupmap);
}

bool UActionInputComponent::SortByValue( pair<unsigned long, ActionData*>& a, pair<unsigned long, ActionData*>& b)
{

	bool isHigher =false;
	
	

	if(a.second->GetData()->Command.size() > b.second->GetData()->Command.size())
	{
		isHigher = true;
	}
	else if(a.second->GetData()->Command.size() == b.second->GetData()->Command.size())
	{
		//커맨드 카운트가 동일시 T->S->H->L->P->J 순으로 우선순위 체크
		std::map<std::string, int> PriorityMap;
		PriorityMap.insert(make_pair("(T)",6));
		PriorityMap.insert(make_pair("(S)",5));
		PriorityMap.insert(make_pair("(H)",4));
		PriorityMap.insert(make_pair("(L)",3));
		PriorityMap.insert(make_pair("(P)",2));
		PriorityMap.insert(make_pair("(J)",1));
		
		int CommandA = 0;
		int CommandB = 0;
		
		for (std::list<string>::iterator it = a.second->GetData()->Command.begin(); it != a.second->GetData()->Command.end(); ++it)
		{
			if(CommandA == 0)
			{
				for (auto iter = PriorityMap.begin(); iter != PriorityMap.end(); iter++)
				{
					if(it->data() == iter->first)
					{
						CommandA = iter->second;
						break;
					}
				}
			}
		}

		for (std::list<string>::iterator it = b.second->GetData()->Command.begin(); it != b.second->GetData()->Command.end(); ++it)
		{
			if(CommandB == 0)
			{
				for (auto iter = PriorityMap.begin(); iter != PriorityMap.end(); iter++)
				{
					if(it->data() == iter->first)
					{
						CommandB = iter->second;
						break;
					}
				}
			}
		}

		if(CommandA > CommandB)
		{
			isHigher = true;
		}

		if(CommandA == CommandB)
		{
			isHigher = true;
		}

		//Map Clear
		PriorityMap.clear();
	}
	
	return isHigher;
}

void UActionInputComponent::FrameViewer()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();

	m_ElapsedTime += CurrentTime;

	if(GEngine)
	{
		FString DebugFrame = "Elapsed Time : "+m_ElapsedTime;
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, *DebugFrame);
		
	}
}

void UActionInputComponent::JumpStart(EJumpState jumpState)
{
	if (m_fRotateValue > 0)
	{
		//float fJump = (float)m_pCharacterDataTable->Jump;
		//float fForwardJump = -(float)m_pCharacterDataTable->Forward_Jump;
		//float fBackJump = (float)m_pCharacterDataTable->Back_Jump;

		float fJump = m_pOwner->TestJumpHeight;
		float fForwardJump = -m_pOwner->TestJumpForward;
		float fBackJump = m_pOwner->TestJumpBackward;

		m_JumpVelocity[(int)EJumpState::E_Vertical].Set(0.0f, 0.0f, fJump);
		m_JumpVelocity[(int)EJumpState::E_Forward].Set(0.0f, fForwardJump, fJump);
		m_JumpVelocity[(int)EJumpState::E_Back].Set(0.0f, fBackJump, fJump);
	}
	else
	{
		//float fJump = (float)m_pCharacterDataTable->Jump;
		//float fForwardJump = (float)m_pCharacterDataTable->Forward_Jump;
		//float fBackJump = -(float)m_pCharacterDataTable->Back_Jump;

		float fJump = m_pOwner->TestJumpHeight;
		float fForwardJump = m_pOwner->TestJumpForward;
		float fBackJump = -m_pOwner->TestJumpBackward;

		m_JumpVelocity[(int)EJumpState::E_Vertical].Set(0.0f, 0.0f, fJump);
		m_JumpVelocity[(int)EJumpState::E_Forward].Set(0.0f, fForwardJump, fJump);
		m_JumpVelocity[(int)EJumpState::E_Back].Set(0.0f, fBackJump, fJump);
	}

	m_pOwner->GetCharacterMovement()->GravityScale = 2.7f;
	m_pOwner->LaunchCharacter(m_JumpVelocity[(int)jumpState], false, false);
}

void UActionInputComponent::JumpAttackCheck()
{
	if (m_ActivateJumpAttack) return;

	auto begin = m_chainActionVec.begin();
	for (; begin != m_chainActionVec.end(); ++begin)
	{
		ActionData* actionData = (*begin);
		if (actionData != nullptr && actionData->ActionCheck(m_pPrevAction, &m_keydownmap, &m_keyupmap) == true)
		{
			ActivateJumpAttackActionNo = actionData->GetData()->SerialNo;
			UE_LOG(LogTemp, Warning, TEXT("SerialNo %ld"), actionData->GetData()->SerialNo);
			//ChangeAction(actionData->GetData()->SerialNo);
			m_ActivateJumpAttack = true;
			return;
		}
	}
}

void UActionInputComponent::ActivateJumpAttack()
{
	if (m_ActivateJumpAttack)
	{
		m_bActionCheck = true;
		ChangeAction(ActivateJumpAttackActionNo);
		m_ActivateJumpAttack = false;
	}
}

void UActionInputComponent::MontageBlendOut(class UAnimMontage* AnimMontage, bool trigger)
{
	if (m_pCurAction == nullptr)
	{
		return;
	}

	if (!trigger)
	{
		m_pCurAction->MontageBlendOut(AnimMontage, trigger);
	}
}
