// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KeyMappingWidgetInterface.h"
#include "Components/InputKeySelector.h"
#include "Framework/Commands/InputChord.h"
#include "InputMappingContext.h"

UKeyMappingWidgetInterface::UKeyMappingWidgetInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}


void UKeyMappingWidgetInterface::CheckCurrentKey(UInputMappingContext* Context)
{
	MappingContext = Context;
	for (FEnhancedActionKeyMapping key : MappingContext->GetMappings())
	{
		if (key.Action == InputAction_Low)
		{
			if (IsValid(InputKeySelector_Low))
			{
				InputKeySelector_Low->SetSelectedKey(key.Key);
				InputKeySelector_Low->OnIsSelectingKeyChanged.AddDynamic(this, &UKeyMappingWidgetInterface::KeySelecting);
				InputKeySelector_Low->OnKeySelected.AddDynamic(this, &UKeyMappingWidgetInterface::KeyChange_Low);
				CurrentKey_Low = key.Key;
			}
		}
		if (key.Action == InputAction_High)
		{
			if (IsValid(InputKeySelector_High))
			{
				InputKeySelector_High->SetSelectedKey(key.Key);
				InputKeySelector_High->OnIsSelectingKeyChanged.AddDynamic(this, &UKeyMappingWidgetInterface::KeySelecting);
				InputKeySelector_High->OnKeySelected.AddDynamic(this, &UKeyMappingWidgetInterface::KeyChange_High);
				CurrentKey_High = key.Key;
			}
		}
		if (key.Action == InputAction_Jump)
		{
			if (IsValid(InputKeySelector_Jump))
			{
				InputKeySelector_Jump->SetSelectedKey(key.Key);
				InputKeySelector_Jump->OnIsSelectingKeyChanged.AddDynamic(this, &UKeyMappingWidgetInterface::KeySelecting);
				InputKeySelector_Jump->OnKeySelected.AddDynamic(this, &UKeyMappingWidgetInterface::KeyChange_Jump);
				CurrentKey_Jump = key.Key;
			}
		}
		if (key.Action == InputAction_Parry)
		{
			if (IsValid(InputKeySelector_Parry))
			{
				InputKeySelector_Parry->SetSelectedKey(key.Key);
				InputKeySelector_Parry->OnIsSelectingKeyChanged.AddDynamic(this, &UKeyMappingWidgetInterface::KeySelecting);
				InputKeySelector_Parry->OnKeySelected.AddDynamic(this, &UKeyMappingWidgetInterface::KeyChange_Parry);
				CurrentKey_Parry = key.Key;
			}
		}
		if (key.Action == InputAction_Throw)
		{
			if (IsValid(InputKeySelector_Throw))
			{
				InputKeySelector_Throw->SetSelectedKey(key.Key);
				InputKeySelector_Throw->OnIsSelectingKeyChanged.AddDynamic(this, &UKeyMappingWidgetInterface::KeySelecting);
				InputKeySelector_Throw->OnKeySelected.AddDynamic(this, &UKeyMappingWidgetInterface::KeyChange_Throw);
				CurrentKey_Throw = key.Key;
			}
		}
		if (key.Action == InputAction_Sp)
		{
			if (IsValid(InputKeySelector_Sp))
			{
				InputKeySelector_Sp->SetSelectedKey(key.Key);
				InputKeySelector_Sp->OnIsSelectingKeyChanged.AddDynamic(this, &UKeyMappingWidgetInterface::KeySelecting);
				InputKeySelector_Sp->OnKeySelected.AddDynamic(this, &UKeyMappingWidgetInterface::KeyChange_Sp);
				CurrentKey_Sp = key.Key;
			}
		}

	}
}


void UKeyMappingWidgetInterface::KeySelecting()
{
	//UE_LOG(LogTemp, Log, TEXT("OnIsSelectingKeyChanged"));
	bListenForRemap = true;
}

void UKeyMappingWidgetInterface::KeyChange_Low(FInputChord SelectedKey)
{
	if (bListenForRemap)
	{
		UE_LOG(LogTemp, Log, TEXT("KeyChange_Low"));
		bListenForRemap = false;
		MappingContext->UnmapKey(InputAction_Low, CurrentKey_Low);
		MappingContext->MapKey(InputAction_Low, SelectedKey.Key);
		CurrentKey_Low = SelectedKey.Key;
	
	}
}

void UKeyMappingWidgetInterface::KeyChange_High(FInputChord SelectedKey)
{
	if (bListenForRemap)
	{
		UE_LOG(LogTemp, Log, TEXT("KeyChange_High"));
		bListenForRemap = false;
		MappingContext->UnmapKey(InputAction_High, CurrentKey_High);
		MappingContext->MapKey(InputAction_High, SelectedKey.Key);
		CurrentKey_High = SelectedKey.Key;

	}
}

void UKeyMappingWidgetInterface::KeyChange_Jump(FInputChord SelectedKey)
{
	if (bListenForRemap)
	{
		UE_LOG(LogTemp, Log, TEXT("KeyChange_Jump"));
		bListenForRemap = false;
		MappingContext->UnmapKey(InputAction_Jump, CurrentKey_Jump);
		MappingContext->MapKey(InputAction_Jump, SelectedKey.Key);
		CurrentKey_Jump = SelectedKey.Key;

	}
}

void UKeyMappingWidgetInterface::KeyChange_Parry(FInputChord SelectedKey)
{
	if (bListenForRemap)
	{
		UE_LOG(LogTemp, Log, TEXT("KeyChange_Parry"));
		bListenForRemap = false;
		MappingContext->UnmapKey(InputAction_Parry, CurrentKey_Parry);
		MappingContext->MapKey(InputAction_Parry, SelectedKey.Key);
		CurrentKey_Parry = SelectedKey.Key;

	}
}

void UKeyMappingWidgetInterface::KeyChange_Throw(FInputChord SelectedKey)
{
	if (bListenForRemap)
	{
		UE_LOG(LogTemp, Log, TEXT("KeyChange_Throw"));
		bListenForRemap = false;
		MappingContext->UnmapKey(InputAction_Throw, CurrentKey_Throw);
		MappingContext->MapKey(InputAction_Throw, SelectedKey.Key);
		CurrentKey_Throw = SelectedKey.Key;

	}
}

void UKeyMappingWidgetInterface::KeyChange_Sp(FInputChord SelectedKey)
{
	if (bListenForRemap)
	{
		UE_LOG(LogTemp, Log, TEXT("KeyChange_Sp"));
		bListenForRemap = false;
		MappingContext->UnmapKey(InputAction_Sp, CurrentKey_Sp);
		MappingContext->MapKey(InputAction_Sp, SelectedKey.Key);
		CurrentKey_Sp = SelectedKey.Key;

	}
}

void UKeyMappingWidgetInterface::MenuPasue()
{
	SetVisibility(ESlateVisibility::Collapsed);
}


