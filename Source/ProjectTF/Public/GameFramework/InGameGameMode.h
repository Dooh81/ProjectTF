// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Core/CCommonController.h"
#include "InGameGameMode.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDele_MULT_WinDecision);
UENUM(BlueprintType)
enum class EWinState : uint8
{
	E_Draw UMETA(DisplayName = "Draw"),
	E_Win1p UMETA(DisplayName = "Win1p"),
	E_Win2p UMETA(DisplayName = "Win2p"),

};


UCLASS()
class PROJECTTF_API AInGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	APawn* Player1P;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	APawn* Player2P;

	UFUNCTION(BlueprintCallable)
	void InitData();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Location1p;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Location2p;

	UPROPERTY(BlueprintAssignable)
		FDele_MULT_WinDecision Fuc_WinCheck;
	UPROPERTY(BlueprintAssignable)
		FDele_MULT_WinDecision Fuc_WinDecision;

	UFUNCTION(BlueprintCallable)
		void InitBattle();
	UFUNCTION(BlueprintCallable)
		void RoundStart();
	UFUNCTION(BlueprintCallable)
		void StartCutScene();
	UFUNCTION(BlueprintCallable)
		void StartBattleReady();

	UFUNCTION(BlueprintCallable)
		void UpperPannelOnOff(ESlateVisibility Visibility);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 WinCount1p;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 WinCount2p;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 TotalWin1p;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 TotalWin2p;
	bool bCanControl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 RoundCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bVisbleHitBoxDebug;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AActor* GlobalCamera;

	int32 RoundRule;
	bool bWinCheck;
	UFUNCTION()
		void WinCheck();

	UFUNCTION()
		void WinDecision();
	class UTokenFighterGameInstance* instance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EWinState WinState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EWinState TotalWinState;
	UFUNCTION(BlueprintCallable)
		void ActiveController(EInGameMode mode,bool CanControl);
	UFUNCTION(BlueprintCallable)
		void TimeUpBattle();

	void DarknessFreezingController(bool CanControl);

	void CheckGameState();

	void SkipIntro();

	UFUNCTION(BlueprintCallable)
	void SetGameStateLoad();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBattleInGameWidget* BattleInGameWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AIngameGameState* IngameState;

	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		bool bDefaultUIOff;

	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		FVector InitCameraPostion;
private:
	FTimerHandle Intro1PTimerHandle;
	FTimerHandle Intro2PTimerHandle;
};
