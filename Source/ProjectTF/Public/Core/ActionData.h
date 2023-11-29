// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <list>
#include "DataTable.h"
#include "PlayerCharacter.h"


using namespace DataTable;

static std::vector<std::string> split(std::string input, char delimiter)
{
	std::vector<std::string> answer;
	std::stringstream ss;
	ss.str(input);
	std::string temp;

	while (std::getline(ss, temp, delimiter)) {
		answer.push_back(temp);
	}

	return answer;
}
/**
 * 
 */
class PROJECTTF_API ActionData
{
public:
	enum MOVE_TYPE
	{
		NONE_MOVE_TYPE = 0,
		FORWARD_MOVE_TYPE = 1,
		RUN_MOVE_TYPE = 2,
		BACK_MOVE_TYPE = 3,
		MAX_MOVE_TYPE = 4,
	};

	enum ROTATE_TYPE
	{
		NONE_ROTATE_TYPE = 0,
		TICK_ROTATE_TYPE = 1,
		ENTER_ROTATE_TYPE = 2,
		MAX_ROATE_TYPE = 3,
	};

	enum CATCH_TYPE
	{
		NONE_CATCH_TYPE = 0,
		USE_CHATCH_TYPE = 1,
		MAX_CATCH_TYPE = 2,
	};

	enum BLOCK_USE_TYPE
	{
		USE_BLOCK_TYPE = 0,
		NOT_USE_BLOCK_TYPE = 1,
		MAX_BLOCK_USE_TYPE = 2,
	};

	enum END_TYPE
	{
		ANI_END = 0,
		COMMAND_END = 1,
		TIME_END = 2,
		PRESS_END = 3,
		MAX_END_TYPE = 4,
	};

	enum PLAYER_STATE
	{
		Stand = 0,
		Crouch = 1,
		Jump = 2,
		Down = 3,
		KO = 4,
	};

	enum ATTACK_STATE
	{
		Idle = 0,
		UpperAttack = 1,
		MiddleAttack = 2, 
		LowerAttack = 3, 
	};

	class CommandData
	{
	public:
		CommandData(std::string commnad, int interval);
		bool CommandCheck(float prevCommandtime, std::map<std::string, float>* pKeyDownMap, std::map<std::string, float>* pKeyUpMap);
		float GetInterval() { return m_interval; }
		std::vector<std::string>* GetCommandVec(){ return &m_commandVec; }
	protected:
		std::vector<std::string> m_commandVec;
		std::vector<float> m_intervalVec;
		bool m_push = false;
		float m_interval = 0.0f;
		float m_multiInterval = 0.0f;
	};
public:
	ActionData(ActionDataTable* dataTable, APlayerCharacter* OwnerPlayer, APlayerCharacter* EnemyPlayer, FOnMontageBlendingOutStarted blendOutDelegate, float RotateValue);
	virtual ~ActionData();

	ActionDataTable* GetData() { return m_pActionDataTable; }
	virtual void Enter();
	virtual void Exit();
	void CheckReset();

	float EnterTime() { return m_enterTime; }
	float ExitTime() { return m_exitTime; }


	bool ActionCheck(ActionData* prevAction, std::map<std::string, float>* pKeyDownMap, std::map<std::string, float>* pKeyUpMap);

	bool EndCheck(std::map<std::string, float>* pKeyDownMap, std::map<std::string, float>* pKeyUpMap);
	unsigned long CancelActionCheck(std::map<std::string, float>* pKeyDownMap, std::map<std::string, float>* pKeyUpMap);

	END_TYPE GetEndType();
	MOVE_TYPE GetMoveType();
	ROTATE_TYPE GetRotateType();
	PLAYER_STATE GetPlayerState();
	ATTACK_STATE GetAttackState();
	bool IsCatchType();
	bool IsBlockType();
	
	virtual void Tick(float DeltaTime);

	APlayerCharacter* m_pOwner;
	APlayerCharacter* m_pEnemy;
	
	void (ActionData::*fp_CurMoveUpdate)(float);
	void (ActionData::*fp_MoveUpdateArray[(int)ActionData::MAX_MOVE_TYPE])(float);
	
	void ForwardMoveUpdate(float deltaTime);
	void RunMoveUpdate(float deltaTime);
	void BackMoveUpdate(float deltaTime);
	
	void RotateUpdate();

	virtual void MontageBlendOut(class UAnimMontage* animmontage, bool trigger);

protected:
	std::vector<CommandData*> m_commandVec;

	ActionDataTable* m_pActionDataTable;

	CharacterDataTable* m_pCharacterDataTable;

	float m_enterTime;
	float m_exitTime;

	int m_current_index = 0;
	float m_current_time = -1.0f;
	float m_fRotateValue;

	UAnimMontage* m_curAnim;

	float ContinueActionIntervalTime;

	FOnMontageBlendingOutStarted m_MontageBlendingOutStartedDelegate;

	
};
