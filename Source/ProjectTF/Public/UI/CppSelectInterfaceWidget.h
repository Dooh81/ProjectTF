// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CppSelectInterfaceWidget.generated.h"

/**
 * 
 */


UCLASS()
class PROJECTTF_API UCppSelectInterfaceWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCppSelectInterfaceWidget(const FObjectInitializer& ObjectInitializer);
	UFUNCTION(BlueprintCallable)
	void SetUIInputUsedTimer(bool bInputable,float time = 0.2f)
	{
		//bUIInputUsed = false;
		if (time >= 0)
		{
			FTimerHandle WaitHandle;
			GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([=]()
				{
					bUIInputUsed = bInputable;
				}), time, false);

		}
	}
	UPROPERTY(BlueprintReadwrite)
		bool bUIInputUsed;

	UPROPERTY(BlueprintReadWrite)
		UUserWidget* OwnerWidget = nullptr;

	UFUNCTION(BlueprintCallable)
		void Init();

	UFUNCTION(BlueprintCallable)
		void SetNextWidget(UCppSelectInterfaceWidget* widget);
	UFUNCTION(BlueprintCallable)
		void GotoNextWidget(int32 Index,float ActiveInputTime = 0.2f);
	UFUNCTION(BlueprintCallable)
		void GotoPrevWidget(float ActiveInputTime = 0.2f);

	UFUNCTION(BlueprintCallable)
		void OpenConfirmWidget(bool bDoOpen_1p = true);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UCppConfirmWidget* ConfirmWidget = nullptr;

	UFUNCTION(BlueprintImplementableEvent)
		void InitSetting();
	UFUNCTION(BlueprintImplementableEvent)
		void ReOpenWidget();
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
		TArray<UCppSelectInterfaceWidget*> NextWidget;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
		UCppSelectInterfaceWidget* PrevWidget = nullptr;


};
