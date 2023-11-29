// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleInGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTF_API UBattleInGameWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UBattleInGameWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UCanvasPanel> UpperPanel;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UCanvasPanel> CanvasP01;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UCanvasPanel> CanvasP02;

	UPROPERTY(EditAnywhere,BlueprintReadwrite)
		int32 IntOfTime = 99;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void BattleReady();

	UFUNCTION(BlueprintCallable)
	void SetPlayingState();

};
