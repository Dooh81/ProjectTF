// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppConfirmWidget.h"


void UCppConfirmWidget::IsOK()
{
	SetVisibility(ESlateVisibility::Collapsed);
	if (Fuc_ConfirmOK.IsBound())
		Fuc_ConfirmOK.Execute();
}

void UCppConfirmWidget::IsCancel()
{
	SetVisibility(ESlateVisibility::Collapsed);
	if (Fuc_ConfirmCancel.IsBound())
		Fuc_ConfirmCancel.Execute();
}
