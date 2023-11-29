// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/IngameGameState.h"

AIngameGameState::AIngameGameState()
{
	InGameState = EInGameState::E_LOAD;
}

void AIngameGameState::SetIngameState(EInGameState State)
{
	InGameState = State;
}
