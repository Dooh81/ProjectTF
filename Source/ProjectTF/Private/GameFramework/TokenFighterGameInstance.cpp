// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/TokenFighterGameInstance.h"

#include "Core/DataTable.h"
#include "Core/DataTableManager.h"

void UTokenFighterGameInstance::Init()
{
	Super::Init();

	SetDefaultTable();
}




void UTokenFighterGameInstance::SetDefaultTable()
{
	GameModeDataTable* GameModeData = static_cast<GameModeDataTable*>(DataTableManager::GetInstance()->GetDataTable("GameMode", 1));
	if(GameModeData)
	{
		RoundTime = GameModeData->RoundTime;
		RoundLimit = GameModeData->RoundLimit;
		
	}

	GlobalSettingDataTable* GlobalSetting = static_cast<GlobalSettingDataTable*>(DataTableManager::GetInstance()->GetDataTable("GlobalSetting", 1));
	if(GlobalSetting)
	{
		MultiCommandInterval = GlobalSetting->MultiCommandInterval /1000;
	}


	int8 CharacterCount = DataTableManager::GetInstance()->GetCharacterCount();

	for (int i= 0; i< CharacterCount; i++)
	{
		CharacterDataTable* CharacterData = static_cast<CharacterDataTable*>(DataTableManager::GetInstance()->GetDataTable("Character", i+1));
		CharacterDataTableList.Add(CharacterData);
	}
}

FString UTokenFighterGameInstance::GetCharacterNameBySerialNum(int32 SerialNum)
{
	if (SerialNum)
	{
		CharacterDataTable* CharacterData = static_cast<CharacterDataTable*>(DataTableManager::GetInstance()->GetDataTable("Character", SerialNum));

		return FString(CharacterData->Name.c_str());
	}
	return "";
}

UTexture2D* UTokenFighterGameInstance::GetCharacterImage(FName CharName, bool bIs1p)
{
	FString URL = CharImageURL + CharName.ToString();
	if (bIs1p)
	{
		URL += "_1p";
	}
	else
	{
		URL += "_2p";
	}

	return LoadObject<UTexture2D>(nullptr, *URL);
}

UTexture2D* UTokenFighterGameInstance::GetCharacterImagePortraite(FName CharName, bool bIs1p)
{
	FString URL = CharImageURL + CharName.ToString() + "_Portraite";
	if (bIs1p)
	{
		URL += "_1p";
	}
	else
	{
		URL += "_2p";
	}
	return LoadObject<UTexture2D>(nullptr, *URL);
}

UTexture2D* UTokenFighterGameInstance::GetCharacterImageSelect(FName CharName)
{
	FString URL = CharImageURL + CharName.ToString() + "_Select";

	return LoadObject<UTexture2D>(nullptr, *URL);
}

UTexture2D* UTokenFighterGameInstance::GetCharacterImageWhite(FName CharName)
{
	FString URL = CharImageURL + CharName.ToString() + "_w";

	return LoadObject<UTexture2D>(nullptr, *URL);
}
