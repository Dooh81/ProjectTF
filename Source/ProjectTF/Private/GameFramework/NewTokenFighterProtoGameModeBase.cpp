// Copyright Epic Games, Inc. All Rights Reserved.


#include "GameFramework/NewTokenFighterProtoGameModeBase.h"
#include "Core/DataTableManager.h"
#include "Core/ActionInputComponent.h"
#include "Kismet/GameplayStatics.h"

ANewTokenFighterProtoGameModeBase::ANewTokenFighterProtoGameModeBase()
{
	
}

void ANewTokenFighterProtoGameModeBase::BeginPlay()
{

	Super::BeginPlay();
	SetGameDefaultforTable();
	UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

void ANewTokenFighterProtoGameModeBase::SetGameDefaultforTable()
{
	GameModeDataTable* GameModeData = static_cast<GameModeDataTable*>(DataTableManager::GetInstance()->GetDataTable("GameMode", 1));
	RoundTime = GameModeData->RoundTime;
	RoundLimit = GameModeData->RoundLimit;
	for (int64 i : GameModeData->StageList)
	{
		StageList.Add(i);
	}

	GlobalSettingDataTable* GlobalSetting = static_cast<GlobalSettingDataTable*>(DataTableManager::GetInstance()->GetDataTable("GlobalSetting", 1));
	if (GlobalSetting->SerialNo == 1)
	{
		MultiCommandInterval = GlobalSetting->MultiCommandInterval / 1000;
	}

	StageDataTable* StageData = static_cast<StageDataTable*>(DataTableManager::GetInstance()->GetDataTable("Stage", 1));
	ListofStageName.Add(StageData->Name.c_str());
	ListofStageBGM.Add(StageData->BGM.c_str());
}

void ANewTokenFighterProtoGameModeBase::GetAllCharacterData()
{
}

void ANewTokenFighterProtoGameModeBase::SpawnCharacterData1P(int64 num)
{
	FActorSpawnParameters SpawnParams;
	FRotator rotator;
	FVector  SpawnLocation;
	if (GetWorld())
	{
		Player1P = GetWorld()->SpawnActor<APlayerCharacter>(PlayerBlueprint[num], SpawnLocation, rotator, SpawnParams);
		//Player1P->CurrentSerialNo = num;
		
		Player1P->CharacterIntialize();

		UE_LOG(LogTemp,Warning,TEXT("Player1P Init"));
	}
}

void ANewTokenFighterProtoGameModeBase::SpawnCharacterData2P(int64 num)
{
	FActorSpawnParameters SpawnParams;
	FRotator rotator;
	FVector  SpawnLocation;
	if (GetWorld())
	{
		Player2P = GetWorld()->SpawnActor<APlayerCharacter>(PlayerBlueprint[num], SpawnLocation, rotator, SpawnParams);
		//Player2P->CurrentSerialNo = num;
		Player2P->CharacterIntialize();

		UE_LOG(LogTemp,Warning,TEXT("Player2P Init"));
		
		Player1P->ActionInputComponent->InitData(Player1P, Player2P, 1.0f);
		Player2P->ActionInputComponent->InitData(Player2P, Player1P, -1.0f);
	}
}
