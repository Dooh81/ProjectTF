// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CCommonController.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Core/PlayerCharacter.h"

ACCommonController::ACCommonController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACCommonController::BeginPlay()
{
	Super::BeginPlay();

	Check();
}

void ACCommonController::Tick(float DeltaTime)
{
}

void ACCommonController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	//{
	//	EnhancedInputComponent->BindAction(InputUp_P01, ETriggerEvent::Started, this, &ACCommonController::P01_UpInput);
	//	EnhancedInputComponent->BindAction(InputUp_P01, ETriggerEvent::Completed, this, &ACCommonController::P01_ResetUpInput);

	//	EnhancedInputComponent->BindAction(CrouchAction_P01, ETriggerEvent::Started, this, &ACCommonController::P01_DownInput);
	//	EnhancedInputComponent->BindAction(CrouchAction_P01, ETriggerEvent::Completed, this, &ACCommonController::P01_ResetDownInput);

	//	EnhancedInputComponent->BindAction(InputRight_P01, ETriggerEvent::Started, this, &ACCommonController::P01_RightInput);
	//	EnhancedInputComponent->BindAction(InputRight_P01, ETriggerEvent::Completed, this, &ACCommonController::P01_ResetRightInput);

	//	EnhancedInputComponent->BindAction(InputLeft_P01, ETriggerEvent::Started, this, &ACCommonController::P01_LeftInput);
	//	EnhancedInputComponent->BindAction(InputLeft_P01, ETriggerEvent::Completed, this, &ACCommonController::P01_ResetLeftInput);

	//	EnhancedInputComponent->BindAction(WeakAction_P01, ETriggerEvent::Started, this, &ACCommonController::P01_WeakInput);
	//	EnhancedInputComponent->BindAction(WeakAction_P01, ETriggerEvent::Completed, this, &ACCommonController::P01_ResetWeakInput);

	//	EnhancedInputComponent->BindAction(StrongAction_P01, ETriggerEvent::Started, this, &ACCommonController::P01_StrongInput);
	//	EnhancedInputComponent->BindAction(StrongAction_P01, ETriggerEvent::Completed, this, &ACCommonController::P01_ResetStrongInput);

	//	EnhancedInputComponent->BindAction(JumpAction_P01, ETriggerEvent::Started, this, &ACCommonController::P01_JumpInput);
	//	EnhancedInputComponent->BindAction(JumpAction_P01, ETriggerEvent::Completed, this, &ACCommonController::P01_ResetJumpInput);

	//	EnhancedInputComponent->BindAction(ParryAction_P01, ETriggerEvent::Started, this, &ACCommonController::P01_ParryInput);
	//	EnhancedInputComponent->BindAction(ParryAction_P01, ETriggerEvent::Completed, this, &ACCommonController::P01_ResetParryInput);

	//	EnhancedInputComponent->BindAction(SpecialAction_P01, ETriggerEvent::Started, this, &ACCommonController::P01_SpecialInput);
	//	EnhancedInputComponent->BindAction(SpecialAction_P01, ETriggerEvent::Completed, this, &ACCommonController::P01_ResetSpecialInput);

	//	EnhancedInputComponent->BindAction(InputUp_P02, ETriggerEvent::Started, this, &ACCommonController::P02_UpInput);
	//	EnhancedInputComponent->BindAction(InputUp_P02, ETriggerEvent::Completed, this, &ACCommonController::P02_ResetUpInput);

	//	EnhancedInputComponent->BindAction(CrouchAction_P02, ETriggerEvent::Started, this, &ACCommonController::P02_DownInput);
	//	EnhancedInputComponent->BindAction(CrouchAction_P02, ETriggerEvent::Completed, this, &ACCommonController::P02_ResetDownInput);

	//	EnhancedInputComponent->BindAction(InputRight_P02, ETriggerEvent::Started, this, &ACCommonController::P02_RightInput);
	//	EnhancedInputComponent->BindAction(InputRight_P02, ETriggerEvent::Completed, this, &ACCommonController::P02_ResetRightInput);

	//	EnhancedInputComponent->BindAction(InputLeft_P02, ETriggerEvent::Started, this, &ACCommonController::P02_LeftInput);
	//	EnhancedInputComponent->BindAction(InputLeft_P02, ETriggerEvent::Completed, this, &ACCommonController::P02_ResetLeftInput);

	//	EnhancedInputComponent->BindAction(WeakAction_P02, ETriggerEvent::Started, this, &ACCommonController::P02_WeakInput);
	//	EnhancedInputComponent->BindAction(WeakAction_P02, ETriggerEvent::Completed, this, &ACCommonController::P02_ResetWeakInput);

	//	EnhancedInputComponent->BindAction(StrongAction_P02, ETriggerEvent::Started, this, &ACCommonController::P02_StrongInput);
	//	EnhancedInputComponent->BindAction(StrongAction_P02, ETriggerEvent::Completed, this, &ACCommonController::P02_ResetStrongInput);

	//	EnhancedInputComponent->BindAction(JumpAction_P02, ETriggerEvent::Started, this, &ACCommonController::P02_JumpInput);
	//	EnhancedInputComponent->BindAction(JumpAction_P02, ETriggerEvent::Completed, this, &ACCommonController::P02_ResetJumpInput);

	//	EnhancedInputComponent->BindAction(ParryAction_P02, ETriggerEvent::Started, this, &ACCommonController::P02_ParryInput);
	//	EnhancedInputComponent->BindAction(ParryAction_P02, ETriggerEvent::Completed, this, &ACCommonController::P02_ResetParryInput);

	//	EnhancedInputComponent->BindAction(SpecialAction_P02, ETriggerEvent::Started, this, &ACCommonController::P02_SpecialInput);
	//	EnhancedInputComponent->BindAction(SpecialAction_P02, ETriggerEvent::Completed, this, &ACCommonController::P02_ResetSpecialInput);
	//}
}

void ACCommonController::Check()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Check")));
}

void ACCommonController::ChangeModeControl(EInGameMode mode)
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if(CurrentMappingContext != NULL)
			Subsystem->RemoveMappingContext(CurrentMappingContext);
		num = (int)mode;
		CurrentMappingContext = MappingContext[num];
		Subsystem->AddMappingContext(CurrentMappingContext, 0);
	}
}

void ACCommonController::P01_UpInput()
{
	if (IsValid(player01))
	{
		player01->UpInput();
	}
}

void ACCommonController::P01_ResetUpInput()
{
	if (IsValid(player01))
	{
		player01->ResetUpInput();
	}

}

void ACCommonController::P01_DownInput()
{
	if (IsValid(player01))
	{
		player01->DownInput();
	}
}

void ACCommonController::P01_ResetDownInput()
{
	if (IsValid(player01))
	{
		player01->ResetDownInput();
	}
}

void ACCommonController::P01_RightInput()
{
	if (IsValid(player01))
	{
		player01->RightInput();
	}
}

void ACCommonController::P01_ResetRightInput()
{
	if (IsValid(player01))
	{
		player01->ResetRightInput();
	}
}

void ACCommonController::P01_LeftInput()
{
	if (IsValid(player01))
	{
		player01->LeftInput();
	}
}

void ACCommonController::P01_ResetLeftInput()
{
	if (IsValid(player01))
	{
		player01->ResetLeftInput();
	}
}

void ACCommonController::P01_WeakInput()
{
	if (IsValid(player01))
	{
		player01->WeakInput();
	}
}

void ACCommonController::P01_ResetWeakInput()
{
	if (IsValid(player01))
	{
		player01->ResetWeakInput();
	}
}

void ACCommonController::P01_StrongInput()
{
	if (IsValid(player01))
	{
		player01->StrongInput();
	}
}

void ACCommonController::P01_ResetStrongInput()
{
	if (IsValid(player01))
	{
		player01->ResetStrongInput();
	}
}

void ACCommonController::P01_JumpInput()
{
	if (IsValid(player01))
	{
		player01->JumpInput();
	}
}

void ACCommonController::P01_ResetJumpInput()
{
	if (IsValid(player01))
	{
		player01->ResetJumpInput();
	}
}

void ACCommonController::P01_ParryInput()
{
	if (IsValid(player01))
	{
		player01->ParryInput();
	}
}

void ACCommonController::P01_ResetParryInput()
{
	if (IsValid(player01))
	{
		player01->ResetParryInput();
	}
}

void ACCommonController::P01_SpecialInput()
{
	if (IsValid(player01))
	{
		player01->SpecialInput();
	}
}

void ACCommonController::P01_ResetSpecialInput()
{
	if (IsValid(player01))
	{
		player01->ResetSpecialInput();
	}
}

void ACCommonController::P02_UpInput()
{
	if (IsValid(player02))
	{
		player02->UpInput();
	}
}

void ACCommonController::P02_ResetUpInput()
{
	if (IsValid(player02))
	{
		player02->ResetUpInput();
	}
}

void ACCommonController::P02_DownInput()
{
	if (IsValid(player02))
	{
		player02->DownInput();
	}
}

void ACCommonController::P02_ResetDownInput()
{
	if (IsValid(player02))
	{
		player02->ResetDownInput();
	}
}

void ACCommonController::P02_RightInput()
{
	if (IsValid(player02))
	{
		player02->RightInput();
	}
}

void ACCommonController::P02_ResetRightInput()
{
	if (IsValid(player02))
	{
		player02->ResetRightInput();
	}
}

void ACCommonController::P02_LeftInput()
{
	if (IsValid(player02))
	{
		player02->LeftInput();
	}
}

void ACCommonController::P02_ResetLeftInput()
{
	if (IsValid(player02))
	{
		player02->ResetLeftInput();
	}
}

void ACCommonController::P02_WeakInput()
{
	if (IsValid(player02))
	{
		player02->WeakInput();
	}
}

void ACCommonController::P02_ResetWeakInput()
{
	if (IsValid(player02))
	{
		player02->ResetWeakInput();
	}
}

void ACCommonController::P02_StrongInput()
{
	if (IsValid(player02))
	{
		player02->StrongInput();
	}
}

void ACCommonController::P02_ResetStrongInput()
{
	if (IsValid(player02))
	{
		player02->ResetStrongInput();
	}
}

void ACCommonController::P02_JumpInput()
{
	if (IsValid(player02))
	{
		player02->JumpInput();
	}
}

void ACCommonController::P02_ResetJumpInput()
{
	if (IsValid(player02))
	{
		player02->ResetJumpInput();
	}
}

void ACCommonController::P02_ParryInput()
{
	if (IsValid(player02))
	{
		player02->ParryInput();
	}
}

void ACCommonController::P02_ResetParryInput()
{
	if (IsValid(player02))
	{
		player02->ResetParryInput();
	}
}

void ACCommonController::P02_SpecialInput()
{
	if (IsValid(player02))
	{
		player02->SpecialInput();
	}
}

void ACCommonController::P02_ResetSpecialInput()
{
	if (IsValid(player02))
	{
		player02->ResetSpecialInput();
	}
}
