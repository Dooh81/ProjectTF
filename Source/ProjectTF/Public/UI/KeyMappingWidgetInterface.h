// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CppSelectInterfaceWidget.h"
#include "KeyMappingWidgetInterface.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTF_API UKeyMappingWidgetInterface : public UCppSelectInterfaceWidget
{
	GENERATED_BODY()
	UKeyMappingWidgetInterface(const FObjectInitializer& ObjectInitializer);
public:

	UFUNCTION(BlueprintCallable)
		void CheckCurrentKey(class UInputMappingContext* Context);
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* TestButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* TestTextBlock;
	
	UPROPERTY(meta = (BindWidget))
		class UInputKeySelector* InputKeySelector_Low;
	UPROPERTY(meta = (BindWidget))
		class UInputKeySelector* InputKeySelector_High;
	UPROPERTY(meta = (BindWidget))
		class UInputKeySelector* InputKeySelector_Jump;
	UPROPERTY(meta = (BindWidget))
		class UInputKeySelector* InputKeySelector_Parry;
	UPROPERTY(meta = (BindWidget))
		class UInputKeySelector* InputKeySelector_Throw;
	UPROPERTY(meta = (BindWidget))
		class UInputKeySelector* InputKeySelector_Sp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InputAction_Low;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InputAction_High;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InputAction_Jump;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InputAction_Parry;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InputAction_Throw;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InputAction_Sp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		struct FKey CurrentKey_Low;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		struct FKey CurrentKey_High;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		struct FKey CurrentKey_Jump;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		struct FKey CurrentKey_Parry;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		struct FKey CurrentKey_Throw;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		struct FKey CurrentKey_Sp;


	UFUNCTION()
	void KeySelecting();
	UFUNCTION()
		void KeyChange_Low(struct FInputChord SelectedKey);
	UFUNCTION()
		void KeyChange_High(struct FInputChord SelectedKey);
	UFUNCTION()
		void KeyChange_Jump(struct FInputChord SelectedKey);
	UFUNCTION()
		void KeyChange_Parry(struct FInputChord SelectedKey);
	UFUNCTION()
		void KeyChange_Throw(struct FInputChord SelectedKey);
	UFUNCTION()
		void KeyChange_Sp(struct FInputChord SelectedKey);

	UFUNCTION(BlueprintCallable)
		void MenuPasue();

	bool bListenForRemap;
};
