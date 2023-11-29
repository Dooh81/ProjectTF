// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Core/DataTable.h"
#include "Core/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NewTokenFighterProtoGameModeBase.generated.h"

using namespace DataTable;

/**
 * 
 */
UCLASS()
class PROJECTTF_API ANewTokenFighterProtoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANewTokenFighterProtoGameModeBase();

public:

protected:
	virtual void BeginPlay() override;

private:
	void SetGameDefaultforTable();


public:
	void GetAllCharacterData();
	TArray<APlayerCharacter> AllCharacterData;

	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "SeialNum"))
		void SpawnCharacterData1P(int64 num);
	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "SeialNum"))
		void SpawnCharacterData2P(int64 num);
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
		TArray<TSubclassOf<APlayerCharacter>> PlayerBlueprint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
		APlayerCharacter* Player1P;
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
		APlayerCharacter* Player2P;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
		int32 RoundTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
		uint8 RoundLimit;
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
		TArray<int64> StageList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
		float MultiCommandInterval;

	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
		TArray<FName> ListofStageName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
		TArray<FName> ListofStageBGM;

	static float GetCurrentTime() {
		return UGameplayStatics::GetTimeSeconds(GEngine->GameViewport->GetWorld());
	}
};
