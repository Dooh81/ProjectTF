// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CCommonController.generated.h"


UENUM(BlueprintType)
enum class EInGameMode : uint8
{
	E_Lobby_1 UMETA(DisplayName = "Lobby_1"),
	E_Lobby_2 UMETA(DisplayName = "Lobby_2"),
	E_Battle_1 UMETA(DisplayName = "Battle_1"),
	E_Battle_2 UMETA(DisplayName = "Battle_2"),
	E_Menu_1 UMETA(DisplayName = "Menu_1"),
	E_Menu_2 UMETA(DisplayName = "Menu_2"),
	E_Intro_1 UMETA(DisplayName = "Intro_1"),
	E_Intro_2 UMETA(DisplayName = "Intro_2"),
};
/**
 * 
 */
UCLASS()
class PROJECTTF_API ACCommonController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACCommonController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input

	virtual void SetupInputComponent() override;
public:
	void Check();
	UFUNCTION(BlueprintCallable)
		void ChangeModeControl(EInGameMode mode);
private:

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Input, meta = (AllowPrivateAccess = "true"))
		TArray<class UInputMappingContext*> MappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* CurrentMappingContext;
	/* 캐릭터1 InputAction */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction_P01;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* CrouchAction_P01;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* WeakAction_P01;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* StrongAction_P01;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* JumpAction_P01;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ParryAction_P01;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* SpecialAction_P01;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InputRight_P01;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InputLeft_P01;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InputUp_P01;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InputThrow_P01;

	/* 캐릭터2 InputAction */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction_P02;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* CrouchAction_P02;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* WeakAction_P02;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* StrongAction_P02;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* JumpAction_P02;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ParryAction_P02;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* SpecialAction_P02;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InputRight_P02;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InputLeft_P02;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InputUp_P02;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InputThrow_P02;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Character, meta = (AllowPrivateAccess = "true"))
		class APlayerCharacter* player01;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Character, meta = (AllowPrivateAccess = "true"))
		class APlayerCharacter* player02;

	/* 캐릭터1 컨트롤 함수 */
	void P01_UpInput();
	void P01_ResetUpInput();
	void P01_DownInput();
	void P01_ResetDownInput();
	void P01_RightInput();
	void P01_ResetRightInput();
	void P01_LeftInput();
	void P01_ResetLeftInput();
	void P01_WeakInput();
	void P01_ResetWeakInput();
	void P01_StrongInput();
	void P01_ResetStrongInput();
	void P01_JumpInput();
	void P01_ResetJumpInput();
	void P01_ParryInput();
	void P01_ResetParryInput();
	void P01_SpecialInput();
	void P01_ResetSpecialInput();
	/* 캐릭터2 컨트롤 함수 */
	void P02_UpInput();
	void P02_ResetUpInput();
	void P02_DownInput();
	void P02_ResetDownInput();
	void P02_RightInput();
	void P02_ResetRightInput();
	void P02_LeftInput();
	void P02_ResetLeftInput();
	void P02_WeakInput();
	void P02_ResetWeakInput();
	void P02_StrongInput();
	void P02_ResetStrongInput();
	void P02_JumpInput();
	void P02_ResetJumpInput();
	void P02_ParryInput();
	void P02_ResetParryInput();
	void P02_SpecialInput();
	void P02_ResetSpecialInput();
	int num;

};
