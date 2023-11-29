// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Core/DataTable.h"
#include "Core/PlayerCharacter.h"
#include "RenderResource.h"
#include "TokenFighterGameInstance.generated.h"

using namespace DataTable;
/**
 * 
 */
UCLASS()
class PROJECTTF_API UTokenFighterGameInstance : public UGameInstance
{
	GENERATED_BODY()
	virtual void Init() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
	TArray<TSubclassOf<APlayerCharacter>> PlayerBlueprint;
	
	UPROPERTY(BlueprintReadwrite)
		APlayerCharacter* Player01;
	UPROPERTY(BlueprintReadwrite)
		APlayerCharacter* Player02;

	UPROPERTY(EditDefaultsOnly,BlueprintReadwrite)
		int32 Player01Index = 3;
	UPROPERTY(EditDefaultsOnly,BlueprintReadwrite)
		int32 Player02Index = 3;
	UPROPERTY(BlueprintReadwrite)
		FName Player01Name = "Bayc01";
	UPROPERTY(BlueprintReadwrite)
		FName Player02Name = "Mayc01";
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
		bool bPlayer01Is1p = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
		bool bActiveController_2p = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
	int32 RoundTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
	uint8 RoundLimit;
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
	TArray<int64> StageList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
	float MultiCommandInterval;
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
	FName StageName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
	TMap<FName,UTexture2D*> MapOfStageName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
	TArray<FName> ListofStageBGM;

	
	void SetDefaultTable();

	void SetCharacterbyTable();
	UFUNCTION(BlueprintCallable, Blueprintpure)
		FString GetCharacterNameBySerialNum(int32 SerialNum);
	UFUNCTION(BlueprintCallable)
		UTexture2D* GetCharacterImage(FName CharName,bool bIs1p);
	UFUNCTION(BlueprintCallable)
		UTexture2D* GetCharacterImagePortraite(FName CharName, bool bIs1p);
	UFUNCTION(BlueprintCallable)
		UTexture2D* GetCharacterImageSelect(FName CharName);
	UFUNCTION(BlueprintCallable)
		UTexture2D* GetCharacterImageWhite(FName CharName);

	int64 CurrentStage;
	TArray<CharacterDataTable*> CharacterDataTableList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
		FString CharImageURL = "/Game/ArtWork/UI/Textures/UIresource/CharacterImage/";
};
