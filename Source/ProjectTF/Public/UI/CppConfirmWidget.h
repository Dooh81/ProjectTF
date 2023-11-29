// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CppConfirmWidget.generated.h"

/**
 * 
 */
DECLARE_DELEGATE(FDele_OpenConfirm);

UCLASS()
class PROJECTTF_API UCppConfirmWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:


	UFUNCTION(BlueprintCallable)
		void IsOK();
	UFUNCTION(BlueprintCallable)
		void IsCancel();

	FDele_OpenConfirm Fuc_ConfirmOK;
	FDele_OpenConfirm Fuc_ConfirmCancel;
	UPROPERTY(BlueprintReadwrite)
		bool bDoOpen_1p = true;
};
