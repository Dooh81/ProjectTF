// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/TFPlayerState.h"

ATFPlayerState::ATFPlayerState()
{
	PS = ETFPlayerState::E_Stand;
	AS = ETFAttackState::E_Idle;
	HS = ETFHitState::E_Idle;
	ADS = ETFAttackDetailState::E_Idle;
	HDS = ETFHitDetailState::E_Idle;
	ES = ETFEnvironmentState::E_Idle;

	CurrentHitState = ETFHitState::E_Idle;
}

void ATFPlayerState::AdditiveJuggleCount(int32 Count)
{
	JuggleCount += Count;
	if(JuggleCount > MaxJuggleCount)
	{
		JuggleCount = MaxJuggleCount;
	}

	UE_LOG(LogTemp,Error,TEXT("CurrentJuggleCount : %d"), JuggleCount);

	if(JuggleCount == MaxJuggleCount)
	{
		UE_LOG(LogTemp,Error,TEXT("MaxJuggleCount Hitter Dropping"));
	}
}

bool ATFPlayerState::IsMaxJuggle() const
{
	if(JuggleCount >= MaxJuggleCount)
	{
		return  true;
	}

	return false;
}

bool ATFPlayerState::IsDisplayComboCount() const
{
	if(ComboCount >= 2)
	{
		return true;
	}
	
	return false;
}
