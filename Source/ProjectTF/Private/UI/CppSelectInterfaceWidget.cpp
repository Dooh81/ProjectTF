// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CppSelectInterfaceWidget.h"
#include "UI/CppConfirmWidget.h"

UCppSelectInterfaceWidget::UCppSelectInterfaceWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UCppSelectInterfaceWidget::SetNextWidget(UCppSelectInterfaceWidget* widget)
{
	NextWidget.Add(widget);
	widget->PrevWidget = this;
}

void UCppSelectInterfaceWidget::GotoNextWidget(int32 Index, float ActiveInputTime)
{
	if (Index >= NextWidget.Num())
		return;
	if (IsValid(NextWidget[Index]))
	{
		NextWidget[Index]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		this->SetVisibility(ESlateVisibility::Collapsed);
		bUIInputUsed = false;
		NextWidget[Index]->SetUIInputUsedTimer(true, ActiveInputTime);
		NextWidget[Index]->InitSetting();
	}
}
void UCppSelectInterfaceWidget::GotoPrevWidget(float ActiveInputTime)
{
	if (IsValid(PrevWidget))
	{
		PrevWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		this->SetVisibility(ESlateVisibility::Collapsed);
		bUIInputUsed = false;
		PrevWidget->SetUIInputUsedTimer(true, ActiveInputTime);
		PrevWidget->ReOpenWidget();
	}
}
void UCppSelectInterfaceWidget::Init()
{
	ConfirmWidget->SetVisibility(ESlateVisibility::Collapsed);
	SetUIInputUsedTimer(true);
}

void UCppSelectInterfaceWidget::OpenConfirmWidget(bool bDoOpen_1p)
{
	bUIInputUsed = false;
	if (IsValid(ConfirmWidget))
	{
		ConfirmWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ConfirmWidget->Fuc_ConfirmOK.BindUFunction(this, FName("GotoPrevWidget"));
		ConfirmWidget->Fuc_ConfirmCancel.BindUFunction(this, FName("Init"));
		ConfirmWidget->bDoOpen_1p = bDoOpen_1p;
	}
}