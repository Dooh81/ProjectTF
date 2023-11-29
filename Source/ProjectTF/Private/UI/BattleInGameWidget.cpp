// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BattleInGameWidget.h"

#include "GameFramework/InGameGameMode.h"
#include "GameFramework/IngameGameState.h"
#include "Kismet/GameplayStatics.h"

UBattleInGameWidget::UBattleInGameWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UBattleInGameWidget::SetPlayingState()
{
	AInGameGameMode* IngameGameMode = Cast<AInGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IngameGameMode)
	{
		IngameGameMode->IngameState->SetIngameState(EInGameState::E_PLAYING);
	}
}
