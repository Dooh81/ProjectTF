#pragma once

#include "CoreMinimal.h"
#include "DataTable.h"
#include "ActionData.h"
#include "PlayerCharacter.h"



class PROJECTTF_API ChainDamagedActionData : public ActionData
{

public:
	enum EChainDamagedState
	{
		E_Start = 0,
		E_Loop,
		E_End
	};
	
public:
	ChainDamagedActionData(ActionDataTable* dataTable, APlayerCharacter* OwnerPlayer, APlayerCharacter* EnemyPlayer, FOnMontageBlendingOutStarted blendOutDelegate, float RotateValue);

	void Enter() override;
	virtual void Exit() override;

	virtual void Tick(float DeltaTime) override;

	virtual void MontageBlendOut(class UAnimMontage* animmontage, bool trigger) override;
	int ComboCount;
	
protected:
	EChainDamagedState ChainDamagedState;
	EChainDamagedType ChainDamagedType;

	FVector ChainDamagedStartDistance;
};
