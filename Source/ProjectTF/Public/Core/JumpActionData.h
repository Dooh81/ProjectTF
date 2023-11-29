// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTable.h"
#include "ActionData.h"
#include "PlayerCharacter.h"

/**
 * 
 */
class PROJECTTF_API JumpActionData : public ActionData
{
public:
	enum EJumpActionState
	{
		E_Start = 0,
		E_Loop,
		E_End
	};

public:
	JumpActionData(ActionDataTable* dataTable, APlayerCharacter* OwnerPlayer, APlayerCharacter* EnemyPlayer, FOnMontageBlendingOutStarted blendOutDelegate, float RotateValue);

	void Enter() override;
	virtual void Exit() override;

	virtual void Tick(float DeltaTime) override;

	virtual void MontageBlendOut(class UAnimMontage* animmontage, bool trigger) override;

protected:
	EJumpActionState m_jumpActionState;
};
