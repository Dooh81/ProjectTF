// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/InGameGameMode.h"
#include "GameFramework/TokenFighterGameInstance.h"
#include "Core/GlobalCameraActor.h"
#include "Core/ActionInputComponent.h"
#include "Core/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "UI/BattleInGameWidget.h"
#include "Components/CanvasPanel.h"
#include "Core/ParticleActor.h"
#include "GameFramework/IngameGameState.h"

void AInGameGameMode::InitData()
{
	instance = Cast<UTokenFighterGameInstance>(GetGameInstance());
	TArray<AActor*> PlayerStart;
	TSubclassOf<APlayerStart> ClassToFind;
	ClassToFind = APlayerStart::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, PlayerStart);
	if(!IsValid(instance->Player01))
	{
		FActorSpawnParameters SpawnParams;
		FRotator rotator;
		FVector  SpawnLocation;
		if (PlayerStart.Num() != 0)
		{
			if (PlayerStart.Num() > 1)
			{
				Location1p = PlayerStart[0]->GetActorLocation();
				rotator = PlayerStart[0]->GetActorRotation();
			}
		}
		instance->Player01 = GetWorld()->SpawnActor<APlayerCharacter>(instance->PlayerBlueprint[instance->Player01Index], Location1p, rotator, SpawnParams);
	}
	if (!IsValid(instance->Player02))
	{
		FActorSpawnParameters SpawnParams;
		FRotator rotator;
		FVector  SpawnLocation;
		if (PlayerStart.Num() != 0)
		{
			if (PlayerStart.Num() > 1)
			{
				Location2p = PlayerStart[1]->GetActorLocation();
				rotator = PlayerStart[1]->GetActorRotation();
			}
		}
		instance->Player02 = GetWorld()->SpawnActor<APlayerCharacter>(instance->PlayerBlueprint[instance->Player02Index], Location2p, rotator, SpawnParams);
		instance->Player02->bMirror = !instance->Player02->bMirror;
		instance->Player02->SkillCamera->SetRelativeLocation(FVector(instance->Player02->SkillCamera->GetRelativeLocation().X, -instance->Player02->SkillCamera->GetRelativeLocation().Y, instance->Player02->SkillCamera->GetRelativeLocation().Z));
		instance->Player02->SkillCamera->SetRelativeRotation(FRotator(instance->Player02->SkillCamera->GetRelativeRotation().Pitch, -instance->Player02->SkillCamera->GetRelativeRotation().Yaw,-instance->Player02->SkillCamera->GetRelativeRotation().Roll));

	}
	instance->Player01->ColIndex = 1;
	instance->Player02->ColIndex = 2;
	if (instance->bPlayer01Is1p)
	{
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(instance->Player01);
		if (!IsValid(UGameplayStatics::GetPlayerController(GetWorld(), 1)))
		{
			UGameplayStatics::CreatePlayer(GetWorld(), 1);
		}
		UGameplayStatics::GetPlayerController(GetWorld(), 1)->Possess(instance->Player02);
	}
	else
	{
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(instance->Player02);
		if (!IsValid(UGameplayStatics::GetPlayerController(GetWorld(), 1)))
		{
			UGameplayStatics::CreatePlayer(GetWorld(), 1);
		}
		UGameplayStatics::GetPlayerController(GetWorld(), 1)->Possess(instance->Player01);

	}

	DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 1));
	
	instance->Player01->BaseStateInitailize();
	instance->Player02->BaseStateInitailize();
	instance->Player01->ActionInputComponent->InitData(instance->Player01, instance->Player02, 1.0f);
	instance->Player02->ActionInputComponent->InitData(instance->Player02, instance->Player01, -1.0f);
	
	
	InitBattle();

	IngameState = GameStateClass->GetDefaultObject<AIngameGameState>();
	if(IngameState != nullptr)
	{
		IngameState->SetIngameState(EInGameState::E_LOAD);
	}
	InitCameraPostion = GlobalCamera->GetActorLocation();
}

void AInGameGameMode::InitBattle()
{
	instance->Player01->SetActorLocation(Location1p);
	instance->Player02->SetActorLocation(Location2p);
	instance->Player01->BaseStateInitailize();
	instance->Player02->BaseStateInitailize();
	instance->Player01->CharacterIntialize();
	instance->Player02->CharacterIntialize();
	instance->Player01->Fuc_BattleLose.BindUFunction(this, FName("WinCheck"));
	instance->Player02->Fuc_BattleLose.BindUFunction(this, FName("WinCheck"));
	
	AGlobalCameraActor* GlobalCameraActor = Cast<AGlobalCameraActor>(GlobalCamera);
	instance->Player01->SettingCutScenePlayerCamera(GlobalCameraActor->CameraComp->PostProcessSettings);
	instance->Player02->SettingCutScenePlayerCamera(GlobalCameraActor->CameraComp->PostProcessSettings);
	GlobalCameraActor->SetActorLocation(InitCameraPostion);
	UpperPannelOnOff(ESlateVisibility::SelfHitTestInvisible);
	GlobalCamera->SetActorLocation(FVector(600, 0, 150));
}

void AInGameGameMode::ActiveController(EInGameMode mode, bool CanControl)
{
	Cast<ACCommonController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->ChangeModeControl(mode);
	Cast<ACCommonController>(UGameplayStatics::GetPlayerController(GetWorld(), 1))->ChangeModeControl(mode);
	if (CanControl)
	{
		EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 1));
		if (instance->bActiveController_2p)
		{
		}
	}
	else
	{
		DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 1));

		//instance->Player01->DisableInput(Cast<APlayerController>(instance->Player01->GetController()));
		//instance->Player02->DisableInput(Cast<APlayerController>(instance->Player02->GetController()));
	}
}

void AInGameGameMode::TimeUpBattle()
{
	
	WinCheck();
}

void AInGameGameMode::DarknessFreezingController(bool CanControl)
{
	// EInGameMode Mode;
	// if(CanControl)
	// {
	// 	instance->Player02->ResetInput();
	// 	instance->Player02->GetMesh()->GlobalAnimRateScale = 0.0f;
	// 	instance->Player02->CustomTimeDilation =0.0f;
	// 	UGameplayStatics::GetPlayerController(GetWorld(), 1)->UnPossess();
	// 	Mode = EInGameMode::E_Intro_1;
	// }
	// else
	// {
	// 	instance->Player02->GetMesh()->GlobalAnimRateScale = 1.0f;
	// 	instance->Player02->CustomTimeDilation =1.0f;
	// 	UGameplayStatics::GetPlayerController(GetWorld(), 1)->Possess(instance->Player02);
	// 	Mode = EInGameMode::E_Battle_1;
	// }
	
	//ActiveController(Mode, !CanControl);
}

void AInGameGameMode::CheckGameState()
{
	if(IngameState->GetIngameState() == EInGameState::E_INTRO)
	{
		//skip intro
		SkipIntro();
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("ingamestate: %hhd"), IngameState->GetIngameState());
	}
	
}

void AInGameGameMode::SkipIntro()
{
	IngameState->SetIngameState(EInGameState::E_INTROSKIP);
	
	float delayTime = 1.0f;

	UGameplayStatics::GetPlayerCameraManager(GetWorld(),0)->StartCameraFade(0,1,1.0f, FLinearColor::White, false, true);

	FTimerHandle FadeTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(FadeTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		GetWorld()->GetTimerManager().ClearTimer(Intro1PTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(Intro2PTimerHandle);

		instance->Player01->ActionInputComponent->ChangeAction(instance->Player01->GetSerialNo() * 10000 + 1);
		instance->Player02->ActionInputComponent->ChangeAction(instance->Player02->GetSerialNo() * 10000 + 1);
		instance->Player01->Darkness(false);

		TArray<AActor*> AllActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AParticleActor::StaticClass(), AllActors);

		for (AActor* Actor : AllActors)
		{
			Cast<AParticleActor>(Actor)->Destroy();
		}
	}), delayTime, false);

	FTimerHandle Fade2TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(Fade2TimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraFade(1, 0, 1.0f, FLinearColor::White);
		}), delayTime * 2, false);

	FTimerHandle Fade3TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(Fade3TimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			StartBattleReady();
		}), delayTime * 3, false);
}

void AInGameGameMode::SetGameStateLoad()
{
	IngameState->SetIngameState(EInGameState::E_LOAD);
}

void AInGameGameMode::RoundStart()
{
	ActiveController(EInGameMode::E_Battle_1, true);
}

void AInGameGameMode::StartCutScene()
{
	IngameState->SetIngameState(EInGameState::E_INTRO);
	RoundStart();
	
	UpperPannelOnOff(ESlateVisibility::Collapsed);
	float IntroDelay1P = instance->Player01->GetAnimIntroTime(71);
	float IntroDelay2P = instance->Player02->GetAnimIntroTime(71);

	UE_LOG(LogTemp,Error,TEXT("IntroDelay1P : %f"), IntroDelay1P);
	UE_LOG(LogTemp,Error,TEXT("IntroDelay2P : %f"), IntroDelay2P);

	instance->Player01->PlayIntro();
	GetWorld()->GetTimerManager().SetTimer(Intro1PTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		instance->Player02->PlayIntro();
		
	}), IntroDelay1P, false);

	GetWorld()->GetTimerManager().SetTimer(Intro2PTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		StartBattleReady();
	}), IntroDelay1P + IntroDelay2P, false);
}

void AInGameGameMode::StartBattleReady()
{
	ActiveController(EInGameMode::E_Intro_1, false);
	UpperPannelOnOff(ESlateVisibility::SelfHitTestInvisible);
	BattleInGameWidget->BattleReady();
}

void AInGameGameMode::UpperPannelOnOff(ESlateVisibility Visibility)
{
	if (IsValid(BattleInGameWidget))
	{
		if(!bDefaultUIOff)
		{
			BattleInGameWidget->UpperPanel->SetVisibility(Visibility);
		}
		else
		{
			BattleInGameWidget->UpperPanel->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}


void AInGameGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AInGameGameMode::WinCheck()
{
	IngameState->SetIngameState(EInGameState::E_END);
	
	ActiveController(EInGameMode::E_Intro_1, false);
	
	instance->Player01->ResetInput();
	instance->Player02->ResetInput();

	if(instance->Player01->GetMesh()->GetAnimInstance()->GetActiveMontageInstance() !=nullptr)
	{
		instance->Player01->GetMesh()->GetAnimInstance()->GetActiveMontageInstance()->SetPlayRate(0.5f);
	}
	if(instance->Player02->GetMesh()->GetAnimInstance()->GetActiveMontageInstance() !=nullptr)
	{
		instance->Player02->GetMesh()->GetAnimInstance()->GetActiveMontageInstance()->SetPlayRate(0.5f);
	}
	//GetWorldSettings()->SetTimeDilation(0.5f);
	
	FTimerHandle TimerDilationHandle2;
	GetWorld()->GetTimerManager().SetTimer(TimerDilationHandle2, FTimerDelegate::CreateLambda([&]()
	{
		if(instance->Player01->GetMesh()->GetAnimInstance()->GetActiveMontageInstance() !=nullptr)
		{
			instance->Player01->GetMesh()->GetAnimInstance()->GetActiveMontageInstance()->SetPlayRate(1.0f);
		}

		if(instance->Player02->GetMesh()->GetAnimInstance()->GetActiveMontageInstance() !=nullptr)
		{
			instance->Player02->GetMesh()->GetAnimInstance()->GetActiveMontageInstance()->SetPlayRate(1.0f);
		}
	}), 1.0f, false);
	
	if (!bWinCheck)
	{
		bWinCheck = true;
		if (instance->Player01->CurrentHp > instance->Player02->CurrentHp)
		{
			++WinCount1p;
			if(instance->Player01->EndGameMoving())
			{
				instance->Player01->GetMesh()->GetAnimInstance()->StopAllMontages(0.0f);
			}

			if(instance->Player02->EndGameMoving())
			{
				instance->Player02->GetMesh()->GetAnimInstance()->StopAllMontages(0.0f);
			}
			
			instance->Player01->ActionInputComponent->GetCurrentAction()->GetData()->NextActionNo = instance->Player01->GetSerialNo()* 10000+1;
		}
		else if (instance->Player02->CurrentHp > instance->Player01->CurrentHp)
		{
			++WinCount2p;

			if(instance->Player01->EndGameMoving())
			{
				instance->Player01->GetMesh()->GetAnimInstance()->StopAllMontages(0.0f);
			}
			
			if(instance->Player02->EndGameMoving())
			{
				instance->Player02->GetMesh()->GetAnimInstance()->StopAllMontages(0.0f);
			}
			
			instance->Player02->ActionInputComponent->GetCurrentAction()->GetData()->NextActionNo = instance->Player02->GetSerialNo()* 10000+1;
		}
		else if(instance->Player01->CurrentHp == instance->Player02->CurrentHp)
		{
			++WinCount1p;
			++WinCount2p;
			
			if(instance->Player01->EndGameMoving())
			{
				instance->Player01->GetMesh()->GetAnimInstance()->StopAllMontages(0.0f);
			}
			
			instance->Player01->ActionInputComponent->GetCurrentAction()->GetData()->NextActionNo = instance->Player01->GetSerialNo()* 10000+1;

			if(instance->Player02->EndGameMoving())
			{
				instance->Player02->GetMesh()->GetAnimInstance()->StopAllMontages(0.0f);
			}
			
			instance->Player02->ActionInputComponent->GetCurrentAction()->GetData()->NextActionNo = instance->Player02->GetSerialNo()* 10000+1;
		}
		
		RoundCount++;	
		if (Fuc_WinCheck.IsBound())
			Fuc_WinCheck.Broadcast();
		Fuc_WinCheck.Clear();
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AInGameGameMode::WinDecision, 2.5f, false);

	}
}

void AInGameGameMode::WinDecision()
{
	if (bWinCheck)
	{
		if (instance->Player01->CurrentHp > instance->Player02->CurrentHp)
		{
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]() {
				instance->Player01->WinBattle();
				ActiveController(EInGameMode::E_Intro_1, true);

				}), 2.5f, false);
			WinState = EWinState::E_Win1p;
		}
		else if (instance->Player02->CurrentHp > instance->Player01->CurrentHp)
		{
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]() {
				instance->Player02->WinBattle();
				ActiveController(EInGameMode::E_Intro_1, true);

				}), 2.5f, false);
			WinState = EWinState::E_Win2p;
		}
		else
		{
			WinState = EWinState::E_Draw;
		}
		if (Fuc_WinDecision.IsBound())
			Fuc_WinDecision.Broadcast();
		Fuc_WinDecision.Clear();
		bWinCheck = false;
	}

}
