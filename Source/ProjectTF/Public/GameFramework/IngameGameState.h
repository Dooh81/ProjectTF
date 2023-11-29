// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "IngameGameState.generated.h"


UENUM(BlueprintType)
enum class EInGameState : uint8
{
	E_LOAD = 0 UMETA(DisplayName = "LOAD"),
	E_INTRO UMETA(DisplayName = "INTRO"),
	E_INTROSKIP UMETA(DisplayName = "INTROSKIP"),
	E_PLAYING UMETA(DisplayName = "PLAYING"),
	E_PAUSE UMETA(DisplayName = "PAUSE"),
	E_END UMETA(DisplayName = "END"),
};

UCLASS()
class PROJECTTF_API AIngameGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	AIngameGameState();

	UFUNCTION()
	void SetIngameState(EInGameState State);

	UFUNCTION()
	EInGameState GetIngameState( ){return InGameState;}
private:
	EInGameState InGameState;

};
