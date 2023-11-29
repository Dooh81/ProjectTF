// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <queue>

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionData.h"
#include "DataTable.h"
#include "Animation/JumpStartNotify.h"
#include "ActionInputComponent.generated.h"

UENUM(BlueprintType)
enum class EInputKey : uint8
{
	KEY_1 = 0 UMETA(DisplayName = "KEY_1"),
	KEY_2 UMETA(DisplayName = "KEY_2"),
	KEY_3 UMETA(DisplayName = "KEY_3"),
	KEY_4 UMETA(DisplayName = "KEY_4"),
	KEY_5 UMETA(DisplayName = "KEY_5"),
	KEY_6 UMETA(DisplayName = "KEY_6"),
	KEY_7 UMETA(DisplayName = "KEY_7"),
	KEY_8 UMETA(DisplayName = "KEY_8"),
	KEY_9 UMETA(DisplayName = "KEY_9"),
	KEY_Low UMETA(DisplayName = "KEY_Low"),
	KEY_High UMETA(DisplayName = "KEY_High"),
	KEY_Jump UMETA(DisplayName = "KEY_Jump"),
	KEY_Parry UMETA(DisplayName = "KEY_Parry"),
	KEY_Throw UMETA(DisplayName = "KEY_Throw"),
	KEY_Special UMETA(DisplayName = "KEY_Special"),
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTTF_API UActionInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActionInputComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	bool ActionCheck();

public:	

	virtual void BeginDestroy() override;

	void InputDownAction(std::string key);
	void InputDownAction(EInputKey key);
	void InputUpAction(std::string key);
	void InputUpAction(EInputKey key);
	void ChangeAction(unsigned long serialNo);
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartActionCheck() { m_bActionCheck = true; }
	void EndActionCheck() { m_bActionCheck = false; }
	void ChangeActionAnimation();

	ActionData* GetCurrentAction() { return m_pCurAction; }
	ActionData* GetActionDataByNum(unsigned long serialNo);
	float GetActionMontageTime(unsigned long serialNo);

	bool GameStateEnd();

	float GetRotateValue() { return m_fRotateValue; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetGuardValue();

	bool GuardCheck(long AttackPosition);

	void SetRotateValue(){m_fRotateValue *= -1.f;}

	void InitData(APlayerCharacter* PlayerCharacter, APlayerCharacter* EnemyCharacter, float RotateValue);
	
	void JumpStart(EJumpState jumpState);

	void JumpAttackCheck();

	void ActivateJumpAttack();

	void MontageBlendOut(class UAnimMontage* AnimMontage, bool trigger);

	void ChangeRotateInput();
	
	void InitailizeInput();

	void ActionClear() { m_keydownmap.clear(); m_keyupmap.clear(); }

	void CommandDebug();

	void HitOrGuardStop(bool GuardState);

	void StiffStop(APlayerCharacter* AttackCaster, float StunTime, bool isAttacker);

	void StiffStopActionSender(float MoveBackTime);

	void StunBackMoveDistance(FVector TicMoveLocation);

	void SetAttackState(EAttackerState AtkState);

	void SetLaucherState(ELauncherState LauncherState);

	void SetLauncherSerialNo(const ulong_t& AppendAction);
	void ClearAppendAction();
	void SetDefaultLowAttack();
	void SetDefaultLowAttackEnemy();
	void EndCombo();
	
	bool IsStand();

	unsigned long CancelActionCheck();

	static bool SortByValue(pair<unsigned long, ActionData*> &a, pair<unsigned long, ActionData*>&b);

	void FrameViewer();

protected:
	std::map<unsigned long, ActionData*> m_actionMap;
	std::map < std::string, float > m_keydownmap;
	std::map < std::string, float > m_keyupmap;
	std::queue<std::string> m_commandQueue;
	std::unordered_map<unsigned long, ActionData*> m_sortMap;

	ActionData* m_pCurAction = nullptr;
	ActionData* m_pPrevAction = nullptr;
	ActionData* m_pDefaultAction = nullptr;

	ulong_t m_pAppendActionNo;
	
	std::vector<ActionData*> m_chainActionVec;

	CharacterDataTable* m_pCharacterDataTable;

	bool m_bActionCheck = false;
	UPROPERTY(VisibleAnywhere,BlueprintReadwrite)
		FString AnimName;
	std::list<ulong_t> Chain_Atk_List;

	APlayerCharacter* m_pOwner;
	APlayerCharacter* m_pEnemy;

	float m_fWalkSpeed;
	float m_fDashSpeed;
	float m_fBackSpeed;

	FVector m_JumpVelocity[(int)EJumpState::E_Back + 1];

	std::string m_keyArray[(int)EInputKey::KEY_Special + 1];

	float m_fRotateValue;

	float ActionCheckTime;

	bool m_ActivateJumpAttack = false;
	unsigned long ActivateJumpAttackActionNo;

	bool m_bGuardCheck = false;

	int m_ElapsedTime = 0;
	
	
	FTimerHandle CommandQueueTimerHandle;

	FTimerHandle StunStopHandle;
	FTimerDelegate SendParameterDelegate;

	FTimerHandle StunBackHandle;
	
	FTimerHandle StunBackMoveHandle;
	FTimerDelegate StunBackMoveSendParameterDelegate;

	FTimerHandle StunBackMoveEndHandle;
};