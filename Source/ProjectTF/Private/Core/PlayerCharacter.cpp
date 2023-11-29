// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PlayerCharacter.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/SlateWrapperTypes.h"
#include "GameFramework/NewTokenFighterProtoGameModeBase.h"
#include "Core/ActionInputComponent.h"
#include "GameFramework/TokenFighterGameInstance.h"
#include "Core/HitBoxComponent.h"
#include "GameFramework/InGameGameMode.h"
#include "Core/ActionData.h"
#include "Core/CombatComponent.h"
#include "Core/AdogenProjectile.h"
#include "Core/Commons.h"
#include "Core/GlobalCameraActor.h"
#include "Kismet/KismetSystemLibrary.h"


#define STAND 1
#define KO_H 20
#define RESULT_WIN 21
#define DAMAGED_CR_AIR_H_START 58
#define DAMAGED_CR_AIR_H_END 60
#define DAMAGED_ST_HIGH_L 65
#define DAMAGED_BLOWWALL 70
#define INTRO_CUT 71
#define DAMAGED_WALL_HIT 72

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ActionInputComponent = CreateDefaultSubobject<UActionInputComponent>(TEXT("ActionInputComponent"));
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));

	HitBoxComponents.Empty();
	DamageBoxComponents.Empty();
	ThrowHitBoxComponents.Empty();
	ThrowDamageBoxComponents.Empty();
	PushWeakBoxComponents.Empty();
	PushStrongBoxComponents.Empty();

	CutSceneCamera = CreateDefaultSubobject<UChildActorComponent>(TEXT("Camera"));
	CutSceneCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
	                                  TEXT("Camera_Socket"));

	SkillCamera = CreateDefaultSubobject<UChildActorComponent>(TEXT("SkillCamera"));
	SkillCamera->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(1.0f, 0.0f, 0.0f));

	WallBlockObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	//


	//PlayerCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PlayerCapsuleComponent"));
	//PlayerCapsuleComponent->OnComponentHit.AddDynamic(this, &APlayerCharacter::OnPlayerHit);
	//PlayerCapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnPlayerOverlapBegin);
	//PlayerCapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnPlayerOverlapEnd);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();


	for (int i = 0; i < GetMesh()->GetMaterials().Num(); i++)
	{
		UMaterialInstanceDynamic* DynamicMaterial =
			UMaterialInstanceDynamic::Create(GetMesh()->GetMaterials()[i], this);
		DynamicMirrorMaterials.Add(DynamicMaterial);
	}


	for (int i = 0; i < GetMesh()->GetMaterials().Num(); i++)
	{
		GetMesh()->SetMaterial(i, DynamicMirrorMaterials[i]);
	}
}

void APlayerCharacter::CharacterIntialize()
{
	for (UHitBoxComponent* HitBoxComponent : HitBoxComponents)
	{
		HitBoxComponent->ColIndex = ColIndex;
		HitBoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1,
		                                               ECollisionResponse::ECR_Ignore);
	}

	for (UHitBoxComponent* DamageBoxComponent : DamageBoxComponents)
	{
		DamageBoxComponent->ColIndex = ColIndex;
	}
	for (UHitBoxComponent* PushWeakBoxComponent : PushWeakBoxComponents)
	{
		PushWeakBoxComponent->ColIndex = ColIndex;
	}
	for (UHitBoxComponent* PushStrongBoxComponent : PushStrongBoxComponents)
	{
		PushStrongBoxComponent->ColIndex = ColIndex;
	}

	UTokenFighterGameInstance* FighterGameInstance = Cast<UTokenFighterGameInstance>(GetGameInstance());
	if (FighterGameInstance)
	{
		MaxHp = FighterGameInstance->CharacterDataTableList[GetSerialNo() - 1]->HP;
		MaxSp = FighterGameInstance->CharacterDataTableList[GetSerialNo() - 1]->MP;

		// UE_LOG(LogTemp, Warning, TEXT("Init HP : %f"), MaxHp);
		// UE_LOG(LogTemp, Warning, TEXT("Init MP : %f"), MaxSp);
	}

	CurrentHp = MaxHp;

	CurrentSp = 0;
	CurrentGp = MaxGp;

	AttackerState = EAttackerState::E_Idle;
	LauncherState = ELauncherState::E_Idle;
	HitterState = EHitterState::E_Idle;
	ActionInputComponent->ClearAppendAction();

	ResetInput();
	ActiveDamegeBox(EStateofDamageBox::E_Idle);
	StartActionCheck();
	ActionInputComponent->ChangeAction(CurrentSerialNo * 10000 + STAND);
	ActionInputComponent->InitailizeInput();
	bBlockWall = false;
	//GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel7);
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel7, ECR_Block);
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel8, ECR_Ignore);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PushWeakBoxComponents[0]->VisibleHitBoxInGame(false);
	PushStrongBoxComponents[0]->VisibleHitBoxInGame(false);

	if (bUseGuard && CurrentGp > 0)
	{
		CurrentGp -= DeltaTime * DecreseGpValue;
		if (CurrentGp < 0)
			CurrentGp = 0;
	}
	else if (!bUseGuard && CurrentGp < MaxGp)
	{
		CurrentGp += DeltaTime * DecreseGpValue;
		if (CurrentGp > MaxGp)
			CurrentGp = MaxGp;
	}

	if (bPush)
	{
		PushEnemy();
	}

	if (bOverlappedPushBox)
	{
		GetPush(PushType, PushPlayer, PushPlayerHitBox, PushPower * PushPlayer->MoveValue * DeltaTime);
	}

	if (bOverlappedHitBox)
	{
		if (IsValid(HitPlayerHitBox))
		{
			GetDamage(HitDamagedType, HitPlayer, HitPlayerHitBox);
		}
		HitDamagedType = EGetDamagedType::E_None;
		HitPlayer = nullptr;
		HitPlayerHitBox = nullptr;
		bOverlappedHitBox = false;
	}
	if (bOverlappedThrowBox)
	{
		if (IsValid(HitPlayerHitBox))
		{
			GetThrow(HitDamagedType, HitPlayer, HitPlayerHitBox);
		}
		HitDamagedType = EGetDamagedType::E_None;
		HitPlayer = nullptr;
		HitPlayerHitBox = nullptr;
		bOverlappedThrowBox = false;
	}
	if (bChangeTime)
	{
		CustomTimeDilation = CurrentCharacterTime;
		if (GetActorTimeDilation() == CurrentCharacterTime)
			bChangeTime = false;
	}
}

void APlayerCharacter::SetupCapsuleCollision()
{
	PlayerCapsuleComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
	                                          TEXT("Com_PointSocket"));
}

void APlayerCharacter::BaseStateInitailize()
{
	ACCommonController* PlayerController = GetController<ACCommonController>();
	if (PlayerController)
	{
		TFPlayerState = Cast<ATFPlayerState>(PlayerController->PlayerState);
		if (TFPlayerState)
		{
			TFPlayerState->SetPlayerState(ETFPlayerState::E_Stand);
			TFPlayerState->SetAttackState(ETFAttackState::E_Idle);
			TFPlayerState->SetHitState(ETFHitState::E_Idle);
			TFPlayerState->SetAttackDetailState(ETFAttackDetailState::E_Idle);
			TFPlayerState->SetHitDetailState(ETFHitDetailState::E_Idle);
			TFPlayerState->SetEnvironmentState(ETFEnvironmentState::E_Idle);
			TFPlayerState->SetMaxJuggleCount(CombatComponent->GetMaxJuggleCount());
		}
	}
}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(InputUp, ETriggerEvent::Started, this, &APlayerCharacter::UpInput);
		EnhancedInputComponent->BindAction(InputUp, ETriggerEvent::Completed, this, &APlayerCharacter::ResetUpInput);

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayerCharacter::DownInput);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this,
		                                   &APlayerCharacter::ResetDownInput);

		EnhancedInputComponent->BindAction(InputRight, ETriggerEvent::Started, this, &APlayerCharacter::RightInput);
		EnhancedInputComponent->BindAction(InputRight, ETriggerEvent::Completed, this,
		                                   &APlayerCharacter::ResetRightInput);

		EnhancedInputComponent->BindAction(InputLeft, ETriggerEvent::Started, this, &APlayerCharacter::LeftInput);
		EnhancedInputComponent->BindAction(InputLeft, ETriggerEvent::Completed, this,
		                                   &APlayerCharacter::ResetLeftInput);

		EnhancedInputComponent->BindAction(WeakAction, ETriggerEvent::Started, this, &APlayerCharacter::WeakInput);
		EnhancedInputComponent->BindAction(WeakAction, ETriggerEvent::Completed, this,
		                                   &APlayerCharacter::ResetWeakInput);

		EnhancedInputComponent->BindAction(StrongAction, ETriggerEvent::Started, this, &APlayerCharacter::StrongInput);
		EnhancedInputComponent->BindAction(StrongAction, ETriggerEvent::Completed, this,
		                                   &APlayerCharacter::ResetStrongInput);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::JumpInput);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this,
		                                   &APlayerCharacter::ResetJumpInput);

		EnhancedInputComponent->BindAction(ParryAction, ETriggerEvent::Started, this, &APlayerCharacter::ParryInput);
		EnhancedInputComponent->BindAction(ParryAction, ETriggerEvent::Completed, this,
		                                   &APlayerCharacter::ResetParryInput);

		EnhancedInputComponent->BindAction(InputThrow, ETriggerEvent::Started, this, &APlayerCharacter::ThrowInput);
		EnhancedInputComponent->BindAction(InputThrow, ETriggerEvent::Completed, this,
		                                   &APlayerCharacter::ResetThrowInput);

		EnhancedInputComponent->BindAction(SpecialAction, ETriggerEvent::Started, this,
		                                   &APlayerCharacter::SpecialInput);
		EnhancedInputComponent->BindAction(SpecialAction, ETriggerEvent::Completed, this,
		                                   &APlayerCharacter::ResetSpecialInput);

		// EnhancedInputComponent->BindAction(InputCameraChangeBattle, ETriggerEvent::Started, this,
		//                                    &APlayerCharacter::BlueprintCameraChangeBattle);
		// EnhancedInputComponent->BindAction(InputCameraChangeThirdPerson, ETriggerEvent::Started, this,
		//                                    &APlayerCharacter::BlueprintCameraChangeThirdPerson);
		EnhancedInputComponent->BindAction(InputMayCIntro, ETriggerEvent::Started, this,
		                                   &APlayerCharacter::PlayIntro);
		EnhancedInputComponent->BindAction(InputBayCIntro, ETriggerEvent::Started, this,
		                                   &APlayerCharacter::PlayIntro);
	}
}

void APlayerCharacter::ActiveHitBoxByIndex(TArray<struct FBoxStatus> BoxStatus, EUseFinishState finish)
{
	FinishState = finish;
	for (int i = 0; i < HitBoxComponents.Num(); i++)
	{
		if (i < BoxStatus.Num())
		{
			if (IsValid(HitBoxComponents[i]))
			{
				HitBoxComponents[i]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				HitBoxComponents[i]->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1,
				                                                   ECollisionResponse::ECR_Overlap);
				HitBoxComponents[i]->VisibleHitBoxInGame(false);
				HitBoxComponents[i]->SetRelativeLocation(FVector(BoxStatus[i].Center.X * InitMirror(), 0.0f,
				                                                 BoxStatus[i].Center.Y));
				HitBoxComponents[i]->SetBoxExtent(FVector(0.0f, BoxStatus[i].Extent.X, BoxStatus[i].Extent.Y));
			}
		}
		else
		{
			if (IsValid(HitBoxComponents[i]))
			{
				HitBoxComponents[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				HitBoxComponents[i]->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1,
				                                                   ECollisionResponse::ECR_Ignore);
				HitBoxComponents[i]->VisibleHitBoxInGame(true);
			}
		}
	}
}

void APlayerCharacter::ChangePushBox(FBoxStatus BoxStatus)
{
	if (PushStrongBoxComponents.Num() != 0)
	{
		if (IsValid(PushStrongBoxComponents[0]))
		{
			PushStrongBoxComponents[0]->SetRelativeLocation(
				FVector(BoxStatus.Center.X * InitMirror(), 0.0f, BoxStatus.Center.Y));
			PushStrongBoxComponents[0]->SetBoxExtent(FVector(0.0f, BoxStatus.Extent.X, BoxStatus.Extent.Y));
			MatchPushBoxtoCollision();
			PushStrongBoxComponents[0]->VisibleHitBoxInGame(false);
		}
	}
}

void APlayerCharacter::ActivePushBox(bool bActive)
{
	if (!bActive)
	{
		PushStrongBoxComponents[0]->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}
	else
	{
		PushStrongBoxComponents[0]->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	}
	PushStrongBoxComponents[0]->VisibleHitBoxInGame(!bActive);
}

void APlayerCharacter::ActiveFinishState(EUseFinishState finish)
{
	FinishState = finish;
}

void APlayerCharacter::ChangeHitBoxTransform(TArray<struct FBoxStatus> BoxStatus)
{
	for (int i = 0; i < BoxStatus.Num() && i < HitBoxComponents.Num(); i++)
	{
		if (IsValid(HitBoxComponents[i]))
		{
			HitBoxComponents[i]->SetRelativeLocation(FVector(BoxStatus[i].Center.X * InitMirror(), 0.0f,
			                                                 BoxStatus[i].Center.Y));
			HitBoxComponents[i]->SetBoxExtent(FVector(0.0f, BoxStatus[i].Extent.X, BoxStatus[i].Extent.Y));
		}
	}
}

void APlayerCharacter::ChangeDamageBoxTransform(EStateofDamageBox state, TArray<struct FBoxStatus> BoxStatus)
{
	CurrentState = state;
	for (int i = 0; i < DamageBoxComponents.Num(); i++)
	{
		if (i < BoxStatus.Num())
		{
			if (IsValid(DamageBoxComponents[i]))
			{
				if (CurrentState != EStateofDamageBox::E_Invincible)
				{
					DamageBoxComponents[i]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
					DamageBoxComponents[i]->SetCollisionResponseToChannel(
						ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Overlap);
					DamageBoxComponents[i]->VisibleHitBoxInGame(false);
				}
				else
				{
					DamageBoxComponents[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
					DamageBoxComponents[i]->SetCollisionResponseToChannel(
						ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
					DamageBoxComponents[i]->VisibleHitBoxInGame(true);

					GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
				}
				DamageBoxComponents[i]->
					SetRelativeLocation(FVector(BoxStatus[i].Center.X * InitMirror(), 0.0f, BoxStatus[i].Center.Y));
				DamageBoxComponents[i]->SetBoxExtent(FVector(0.0f, BoxStatus[i].Extent.X, BoxStatus[i].Extent.Y));
			}
		}
		else
		{
			if (IsValid(DamageBoxComponents[i]))
			{
				DamageBoxComponents[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				DamageBoxComponents[i]->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3,
				                                                      ECollisionResponse::ECR_Ignore);
				DamageBoxComponents[i]->VisibleHitBoxInGame(true);
			}
		}
	}
}

void APlayerCharacter::DeActiveHitBox()
{
	if (HitBoxComponents.Num() == 0)
		return;
	for (int i = 0; i < HitBoxComponents.Num(); i++)
	{
		if (IsValid(HitBoxComponents[i]))
		{
			HitBoxComponents[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			HitBoxComponents[i]->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1,
			                                                   ECollisionResponse::ECR_Ignore);
			HitBoxComponents[i]->VisibleHitBoxInGame(true);
		}
	}
}

void APlayerCharacter::ActiveDamegeBox(EStateofDamageBox state)
{
	CurrentState = state;

	if (state == EStateofDamageBox::E_Invincible)
	{
		for (int i = 0; i < DamageBoxComponents.Num(); i++)
		{
			DamageBoxComponents[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			DamageBoxComponents[i]->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3,
			                                                      ECollisionResponse::ECR_Ignore);
			DamageBoxComponents[i]->VisibleHitBoxInGame(true);
		}
		return;
	}
	else if (state == EStateofDamageBox::E_Down)
	{
		//EnemyEndCombo();
	}
}

void APlayerCharacter::MatchPushBoxtoCollision()
{
	if (IsValid(PushStrongBoxComponents[0]))
	{
		float radius = abs(PushStrongBoxComponents[0]->GetScaledBoxExtent().Y);
		GetCapsuleComponent()->SetCapsuleRadius(radius - 2, true);
		GetCapsuleComponent()->SetCapsuleHalfHeight(60, true);
	}
}

void APlayerCharacter::AddSP(ActionData* pActionData)
{
	const float AddSp = pActionData->GetData()->MP_Self_Add_OH;
	CurrentSp = CurrentSp + AddSp > MaxSp ? MaxSp : CurrentSp + AddSp;
}

void APlayerCharacter::StartActionCheck()
{
	ActionInputComponent->StartActionCheck();
	bUseGuard = false;
}

void APlayerCharacter::EndActionCheck()
{
	ActionInputComponent->EndActionCheck();
	bUseGuard = false;
}

void APlayerCharacter::EndActionAnimation()
{
	ActionInputComponent->ChangeActionAnimation();
	//bBlockWall = false;
}

void APlayerCharacter::SettingCutScenePlayerCamera(FPostProcessSettings PostProcessSettigs)
{
	ACameraActor* CameraActor = Cast<ACameraActor>(CutSceneCamera->GetChildActor());

	if (CameraActor)
	{
		CameraActor->GetCameraComponent()->PostProcessSettings = PostProcessSettigs;
	}
}

void APlayerCharacter::UpInput()
{
	ActionInputComponent->InputDownAction(EInputKey::KEY_8);
}

void APlayerCharacter::ResetUpInput()
{
	ActionInputComponent->InputUpAction(EInputKey::KEY_8);
}

void APlayerCharacter::DownInput()
{
	ActionInputComponent->InputDownAction(EInputKey::KEY_2);
}

void APlayerCharacter::ResetDownInput()
{
	ActionInputComponent->InputUpAction(EInputKey::KEY_2);
}

void APlayerCharacter::RightInput()
{
	ActionInputComponent->InputDownAction(ActionInputComponent->GetRotateValue() > 0.0f
		                                      ? EInputKey::KEY_6
		                                      : EInputKey::KEY_4);
}

void APlayerCharacter::ResetRightInput()
{
	ActionInputComponent->InputUpAction(ActionInputComponent->GetRotateValue() > 0.0f
		                                    ? EInputKey::KEY_6
		                                    : EInputKey::KEY_4);
}


void APlayerCharacter::LeftInput()
{
	ActionInputComponent->InputDownAction(ActionInputComponent->GetRotateValue() > 0.0f
		                                      ? EInputKey::KEY_4
		                                      : EInputKey::KEY_6);
}

void APlayerCharacter::ResetLeftInput()
{
	ActionInputComponent->InputUpAction(ActionInputComponent->GetRotateValue() > 0.0f
		                                    ? EInputKey::KEY_4
		                                    : EInputKey::KEY_6);
}

void APlayerCharacter::WeakInput()
{
	ActionInputComponent->InputDownAction(EInputKey::KEY_Low);
}

void APlayerCharacter::ResetWeakInput()
{
	ActionInputComponent->InputUpAction(EInputKey::KEY_Low);
}

void APlayerCharacter::StrongInput()
{
	ActionInputComponent->InputDownAction(EInputKey::KEY_High);
}

void APlayerCharacter::ResetStrongInput()
{
	ActionInputComponent->InputUpAction(EInputKey::KEY_High);
}

void APlayerCharacter::JumpInput()
{
	ActionInputComponent->InputDownAction(EInputKey::KEY_Jump);
	//Jump();
}

void APlayerCharacter::ResetJumpInput()
{
	ActionInputComponent->InputUpAction(EInputKey::KEY_Jump);
}

void APlayerCharacter::ParryInput()
{
	ActionInputComponent->InputDownAction(EInputKey::KEY_Parry);
}

void APlayerCharacter::ResetParryInput()
{
	ActionInputComponent->InputUpAction(EInputKey::KEY_Parry);
}

void APlayerCharacter::SpecialInput()
{
	ActionInputComponent->InputDownAction(EInputKey::KEY_Special);
}

void APlayerCharacter::ResetSpecialInput()
{
	ActionInputComponent->InputUpAction(EInputKey::KEY_Special);
}

void APlayerCharacter::ThrowInput()
{
	ActionInputComponent->InputDownAction(EInputKey::KEY_Throw);
}

void APlayerCharacter::ResetThrowInput()
{
	ActionInputComponent->InputUpAction(EInputKey::KEY_Throw);
}

void APlayerCharacter::ResetInput()
{
	ResetUpInput();
	ResetDownInput();
	ResetRightInput();
	ResetLeftInput();
	ResetWeakInput();
	ResetStrongInput();
	ResetJumpInput();
	ResetParryInput();
	ResetSpecialInput();
	ResetThrowInput();
	ActionInputComponent->ActionClear();
}

void APlayerCharacter::CheckBlockingWall()
{
	FVector LineEnd = FVector(0, abs(PushStrongBoxComponents[0]->GetScaledBoxExtent().Y) * InitMirror(), 0);

	TArray<AActor*> IgnoreActors; // 무시할 액터들.

	FHitResult HitResult; // 히트 결과 값 받을 변수.

	bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation() + LineEnd,
		WallBlockObjectTypes,
		false,
		IgnoreActors, // 무시할 것이 없다고해도 null을 넣을 수 없다.
		EDrawDebugTrace::None,
		HitResult,
		true
		// 여기 밑에 3개는 기본 값으로 제공됨. 바꾸려면 적으면 됨.
		//, FLinearColor::Red
		//, FLinearColor::Green
		//, 5.0f
	);


	if (Result) //GetActorLocation().Y > 946 || GetActorLocation().Y < -946)
		bBlockWall = true;
	else
		bBlockWall = false;
}

void APlayerCharacter::Stiff()
{
	if (ActionInputComponent->GetGuardValue())
	{
		GuardStop();
	}
	else
	{
		HitStop();
	}
}

void APlayerCharacter::HitStop()
{
	ActionInputComponent->HitOrGuardStop(false);
	//UE_LOG(LogTemp, Warning, TEXT("HitStop"));
}

void APlayerCharacter::GuardStop()
{
	ActionInputComponent->HitOrGuardStop(true);
	//UE_LOG(LogTemp, Warning, TEXT("GuardStop"));
}

void APlayerCharacter::EnemyEndCombo()
{
	UE_LOG(LogTemp, Error, TEXT("End Combo Count : %d"), TFPlayerState->GetComboCount());
	ActionInputComponent->EndCombo();
	HitterState = EHitterState::E_Idle;
}

void APlayerCharacter::PlayIntro()
{
	ActionInputComponent->ChangeAction(GetSerialNo() * 10000 + INTRO_CUT);
	UE_LOG(LogTemp, Warning, TEXT("PlayIntro Cut No : %d"), GetSerialNo() * 10000 + INTRO_CUT);
}

bool APlayerCharacter::EndGameMoving()
{
	unsigned long MoveAnimSerialNo = ActionInputComponent->GetCurrentAction()->GetData()->SerialNo - GetSerialNo() *
		10000;

	if (MoveAnimSerialNo <= 5)
	{
		return true;
	}

	return false;
}

void APlayerCharacter::MirrorChangeBlendMaterial()
{
	float MirrorG = bMirror ? -0.2f : 0.2f;
	FLinearColor MirrorColor = FLinearColor(-0.4f, MirrorG, 1.8, 0.0);
	for (int i = 0; i < DynamicMirrorMaterials.Num(); i++)
	{
		DynamicMirrorMaterials[i]->SetVectorParameterValue(TEXT("LightAngle"), MirrorColor);
	}
}

void APlayerCharacter::ChangeCameraBattle()
{
	UE_LOG(LogTemp, Warning, TEXT("ChangeCameraBattle"));

	AInGameGameMode* IngameGameMode = Cast<AInGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IngameGameMode)
	{
		if (IngameGameMode->GlobalCamera != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(
				UGameplayStatics::GetPlayerController(GetWorld(), 0));
			if (PlayerController)
			{
				PlayerController->SetViewTargetWithBlend(IngameGameMode->GlobalCamera,
				                                         CombatComponent->fCutSceneCameraBlendTime);
				Darkness(false);
			}
		}
	}
}

void APlayerCharacter::ChangeActionIntro(FString SelectNumber)
{
	if (SelectNumber == "C")
	{
		ActionInputComponent->ChangeAction(CurrentSerialNo * 10000 + INTRO_CUT);
	}
	else if (SelectNumber == "D")
	{
		ActionInputComponent->ChangeAction(CurrentSerialNo * 10000 + INTRO_CUT);
	}
}

//void APlayerCharacter::ShakeCameraAndParticlefromDamage(float Damage, APlayerCharacter* AttackCaster, UHitBoxComponent* HitComponent)
//{
//	TArray<AActor*> GlobalCameraActors;
//	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGlobalCameraActor::StaticClass(), GlobalCameraActors);
//	FVector CameraLocation = GlobalCameraActors[0]->GetActorLocation();
//
//
//}

void APlayerCharacter::DamageCheck(UObject* OtherActor, UPrimitiveComponent* OtherComponent)
{
	UHitBoxComponent* HitBox = Cast<UHitBoxComponent>(OtherComponent);
	if (IsValid(HitBox))
	{
		if (HitBox->ColIndex != ColIndex && HitBox->ColIndex != 0)
		{
			HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
			if (!IsValid(Player))
			{
				AAdogenProjectile* Projectile = Cast<AAdogenProjectile>(OtherActor);
				if (IsValid(Projectile))
				{
					HitDamagedType = EGetDamagedType::E_Ranged;
					Player = Projectile->OwnerPlayer;
				}
			}
			else
			{
				HitDamagedType = EGetDamagedType::E_Melee;
				Player->DeActiveHitBox();
			}
			HitPlayer = Player;
			HitPlayerHitBox = HitBox;
			bOverlappedHitBox = true;
		}
	}
}

void APlayerCharacter::PushCheck(UObject* OtherActor, UPrimitiveComponent* OtherComponent)
{
	UHitBoxComponent* PushBox = Cast<UHitBoxComponent>(OtherComponent);
	if (IsValid(PushBox))
	{
		if (PushBox->ColIndex != ColIndex && PushBox->ColIndex != 0)
		{
			APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
			if (!IsValid(Player))
			{
				AAdogenProjectile* Projectile = Cast<AAdogenProjectile>(OtherActor);
				if (IsValid(Projectile))
				{
					PushType = EGetDamagedType::E_Ranged;
					Player = Projectile->OwnerPlayer;
				}
			}
			PushPlayer = Player;
			PushPlayerHitBox = PushBox;
			bOverlappedThrowBox = true;
		}
	}
}

void APlayerCharacter::ThrowCheck(UObject* OtherActor, UPrimitiveComponent* OtherComponent)
{
	UHitBoxComponent* ThrowBox = Cast<UHitBoxComponent>(OtherComponent);
	if (IsValid(ThrowBox))
	{
		if (ThrowBox->ColIndex != ColIndex && ThrowBox->ColIndex != 0)
		{
			ThrowBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
			if (!IsValid(Player))
			{
				AAdogenProjectile* Projectile = Cast<AAdogenProjectile>(OtherActor);
				if (IsValid(Projectile))
				{
					HitDamagedType = EGetDamagedType::E_Ranged;
					Player = Projectile->OwnerPlayer;
				}
			}
			else
			{
				HitDamagedType = EGetDamagedType::E_Melee;
				Player->DeActiveHitBox();
			}
			HitPlayer = Player;
			HitPlayerHitBox = ThrowBox;
			bOverlappedHitBox = true;
		}
	}
}

void APlayerCharacter::EndPush()
{
	PushType = EGetDamagedType::E_None;
	PushPlayer = nullptr;
	PushPlayerHitBox = nullptr;
	bOverlappedPushBox = false;
}

long APlayerCharacter::GetSerialNo()
{
	return CurrentSerialNo;
}

void APlayerCharacter::GetDamage(EGetDamagedType type, APlayerCharacter* AttackCaster, UHitBoxComponent* HitComponent)
{
	//UE_LOG(LogTemp,Warning,TEXT("Finish : %hhd"), FinishState);

	CheckBlockingWall();
	if (IsValid(AttackCaster))
	{
		const ActionDataTable* AttackData = AttackCaster->ActionInputComponent->GetCurrentAction()->GetData();
		float Damage = 0;
		
		CurrentHp -= Damage;
		CurrentSp = CurrentSp + AttackData->MP_Add_OH > MaxSp ? MaxSp : CurrentSp + AttackData->MP_Add_OH;
		AttackCaster->CurrentSp += AttackData->MP_Self_Add_OH > MaxSp ? MaxSp : AttackData->MP_Self_Add_OH;
		
		GetDamageMakeHitState(AttackCaster);
		DamagedActionPlay();

		if (type == EGetDamagedType::E_Melee)
		{
			AttackCaster->PushWeakBoxComponents[0]->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Overlap);
		}
		else if (type == EGetDamagedType::E_Ranged)
		{
			AttackData = AttackCaster->ActionInputComponent->GetActionDataByNum(AttackCaster->CurrentSerialNo * 10000 + 75)->GetData();
		}
		
		
		bool isGuard = ActionInputComponent->GuardCheck(AttackData->AttackState);
		if (isGuard)
		{
			Damage = AttackData->B_Damage;
			GetPush(type, AttackCaster, HitComponent, AttackData->BlockStun);
		}
		else
		{
			Damage = AttackData->Damage;
			GetPush(type, AttackCaster, HitComponent, AttackData->HitStun);
			
			if (AttackCaster->TFPlayerState->GetAttackDetailState() == ETFAttackDetailState::E_Idle && AttackData->Launcher_Atk == 1)
			{
				AttackCaster->TFPlayerState->SetAttackDetailState(ETFAttackDetailState::E_Launcher);
				AttackCaster->ActionInputComponent->SetLauncherSerialNo(AttackCaster->ActionInputComponent->GetCurrentAction()->GetData()->Chain_Action_Append);
			}
			else if (AttackCaster->TFPlayerState->GetAttackDetailState() == ETFAttackDetailState::E_Launcher)
			{
				int32 JugglePoint = AttackCaster->ActionInputComponent->GetCurrentAction()->GetData()->Juggle;
				AttackCaster->TFPlayerState->AdditiveJuggleCount(JugglePoint);
				AttackCaster->TFPlayerState->AdditiveComboCount();
				if (AttackCaster->TFPlayerState->IsDisplayComboCount())
				{
					FString ComboString = FString::FromInt(AttackCaster->TFPlayerState->GetComboCount());
				}

				if (AttackCaster->TFPlayerState->IsMaxJuggle())
				{
					TFPlayerState->SetHitDetailState(ETFHitDetailState::E_Chain_Juggle_Finish);
				}
			}
		}
		
		float HitInterval = AttackData->HitInterval / 1000.f;
		if (HitInterval > 0)
		{
			ActionInputComponent->StiffStop(AttackCaster, HitInterval, false);
			AttackCaster->ActionInputComponent->StiffStop(this, HitInterval, true);
		}
	
		ShakeCameraAndParticlefromDamage(Damage, AttackCaster, HitComponent);

		LoseBattle(AttackCaster);
		
		UE_LOG(LogTemp, Warning, TEXT("Hit Detail : %s"), *GetEnumerationToFullString(TFPlayerState->GetHitDetailState()));
	}
}

void APlayerCharacter::DamagedActionPlay()
{
	ETFHitDetailState HitDetailState = TFPlayerState->GetHitDetailState();

	switch (HitDetailState)
	{
	case ETFHitDetailState::E_Jump_Weak_Hit:
		
		break;
	case ETFHitDetailState::E_Jump_Strong_Hit:
		
		break;
	case ETFHitDetailState::E_Chain_Juggle_Start:
		
		break;
	case ETFHitDetailState::E_Chain_Juggle_Hit:
		
		break;
	case ETFHitDetailState::E_Chain_Juggle_Finish:
		
		break;
	case ETFHitDetailState::E_Throw_Start:
		
		break;
	case ETFHitDetailState::E_Throw_Success_Hit:
		
		break;
	case ETFHitDetailState::E_Throw_Fail_Tech:
		
		break;
	default:
		//Change Hit Animation
		unsigned long DamagedAnimNo =  CurrentSerialNo * 10000 + static_cast<uint8>(TFPlayerState->GetHitDetailState());
		ActionInputComponent->ChangeAction(DamagedAnimNo);
		break;
	}
}


void APlayerCharacter::GetThrow(EGetDamagedType type, APlayerCharacter* AttackCaster, UHitBoxComponent* HitComponent)
{
	ActionInputComponent->ChangeAction(CurrentSerialNo * 10000 + DAMAGED_WALL_HIT);
}

void APlayerCharacter::GetDamageMakeHitState(APlayerCharacter* Attacker)
{
	//state init
	BaseStateInitailize();

	const ActionDataTable* AttackData = Attacker->ActionInputComponent->GetCurrentAction()->GetData();

	//Guard
	bool isGuard = ActionInputComponent->GuardCheck(static_cast<long>(TFPlayerState->GetAttackState()));

	//Launcher
	bool isLauncher = AttackData->Launcher_Atk == 1 ? true : false;

	//weak, strong 구분
	bool isWeak = AttackData->Damage <= 100 ? true : false;

	//잡기
	bool isCatch = AttackData->CatchType == 1 ? true : false;

	//down 구분
	long DownCheck = AttackData->DownCheck;

	float HP = Attacker->CurrentHp;

	if(HP <= 0 && DownCheck == 0)
	{
		if(isWeak)
		{
			DownCheck = 2;
		}
		else
		{
			DownCheck = 3;
		}
 	}

	if (!isGuard)
	{
		TFPlayerState->SetHitState(ETFHitState::E_Hit);

		if (isLauncher)
		{
			TFPlayerState->SetHitState(ETFHitState::E_Juggle);
		}

		if(isCatch)
		{
			TFPlayerState->SetHitState(ETFHitState::E_Catch);
		}
	}
	else
	{
		TFPlayerState->SetHitState(ETFHitState::E_Guard);
	}

	//Hit, Guard, Juggle, Counter *Idle과 Invincibility 데미지를 입지 않음.
	if (TFPlayerState->GetHitState() == ETFHitState::E_Hit)
	{
		switch (TFPlayerState->GetPlayerState())
		{
		case ETFPlayerState::E_Jump:
			{
				if (isWeak)
				{
					TFPlayerState->SetHitDetailState(ETFHitDetailState::E_Jump_Weak_Hit);
				}
				else
				{
					TFPlayerState->SetHitDetailState(ETFHitDetailState::E_Jump_Strong_Hit);
				}
			}
			break;
		case ETFPlayerState::E_Stand:
			{
				if (Attacker->TFPlayerState->GetAttackState() == ETFAttackState::E_UpperAttack)
				{
					int32 RandNum = FMath::RandRange(1,3);
					if (isWeak)
					{
						if(RandNum == 1)
						{
							TFPlayerState->SetHitDetailState(ETFHitDetailState::E_St_Up_Weak_Hit_1);
						}
						else if(RandNum == 2)
						{
							TFPlayerState->SetHitDetailState(ETFHitDetailState::E_St_Up_Weak_Hit_2);
						}
						else if(RandNum == 3)
						{
							TFPlayerState->SetHitDetailState(ETFHitDetailState::E_St_Up_Weak_Hit_3);
						}
						
					}
					else
					{
						if(RandNum == 1)
						{
							TFPlayerState->SetHitDetailState(ETFHitDetailState::E_St_Up_Strong_Hit_1);
						}
						else if(RandNum == 2)
						{
							TFPlayerState->SetHitDetailState(ETFHitDetailState::E_St_Up_Strong_Hit_2);
						}
						else if(RandNum == 3)
						{
							TFPlayerState->SetHitDetailState(ETFHitDetailState::E_St_Up_Strong_Hit_3);
						}
					}
				}

				if (Attacker->TFPlayerState->GetAttackState() == ETFAttackState::E_MiddleAttack)
				{
					TFPlayerState->SetHitDetailState(ETFHitDetailState::E_St_Mid_Hit);
				}

				if (Attacker->TFPlayerState->GetAttackState() == ETFAttackState::E_LowerAttack)
				{
					if (isWeak)
					{
						TFPlayerState->SetHitDetailState(ETFHitDetailState::E_St_Low_Weak_Hit);
					}
					else
					{
						TFPlayerState->SetHitDetailState(ETFHitDetailState::E_St_Low_Strong_Hit);
					}
				}

				switch (DownCheck)
				{
				case 1:
					TFPlayerState->SetHitDetailState(ETFHitDetailState::E_St_Down_Front);
					break;
				case 2:
					TFPlayerState->SetHitDetailState(ETFHitDetailState::E_St_Down_Back);
					break;
				case 3:
					TFPlayerState->SetHitDetailState(ETFHitDetailState::E_St_Down_Critical);
					break;
				default:
					break;
				}
			}
			break;
		case ETFPlayerState::E_Crouch:
			{
				if (Attacker->TFPlayerState->GetAttackState() == ETFAttackState::E_UpperAttack)
				{
					int32 RandNum = FMath::RandRange(1,3);
					if (isWeak)
					{
						if(RandNum == 1)
						{
							TFPlayerState->SetHitDetailState(ETFHitDetailState::E_Cr_Up_Weak_Hit_1);
						}
						else if(RandNum == 2)
						{
							TFPlayerState->SetHitDetailState(ETFHitDetailState::E_Cr_Up_Weak_Hit_2);
						}
						else if(RandNum == 3)
						{
							TFPlayerState->SetHitDetailState(ETFHitDetailState::E_Cr_Up_Weak_Hit_3);
						}
						
					}
					else
					{
						if(RandNum == 1)
						{
							TFPlayerState->SetHitDetailState(ETFHitDetailState::E_Cr_Up_Strong_Hit_1);
						}
						else if(RandNum == 2)
						{
							TFPlayerState->SetHitDetailState(ETFHitDetailState::E_Cr_Up_Strong_Hit_2);
						}
						else if(RandNum == 3)
						{
							TFPlayerState->SetHitDetailState(ETFHitDetailState::E_Cr_Up_Strong_Hit_3);
						}
					}
				}

				if (Attacker->TFPlayerState->GetAttackState() == ETFAttackState::E_MiddleAttack)
				{
					TFPlayerState->SetHitDetailState(ETFHitDetailState::E_Cr_Mid_Hit);
				}

				if (Attacker->TFPlayerState->GetAttackState() == ETFAttackState::E_LowerAttack)
				{
					if (isWeak)
					{
						TFPlayerState->SetHitDetailState(ETFHitDetailState::E_Cr_Low_Weak_Hit);
					}
					else
					{
						TFPlayerState->SetHitDetailState(ETFHitDetailState::E_Cr_Low_Strong_Hit);
					}
				}

				switch (DownCheck)
				{
				case 1:
					TFPlayerState->SetHitDetailState(ETFHitDetailState::E_Cr_Down_Front);
					break;
				case 2:
					TFPlayerState->SetHitDetailState(ETFHitDetailState::E_Cr_Down_Back);
					break;
				case 3:
					TFPlayerState->SetHitDetailState(ETFHitDetailState::E_Cr_Down_Critical);
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}

		if (TFPlayerState->GetCurrentHitState() == ETFHitState::E_Juggle)
		{
			
			TFPlayerState->SetHitDetailState(ETFHitDetailState::E_Chain_Juggle_Hit);

			if (AttackData->Finish == 1)
			{
				TFPlayerState->SetHitDetailState(ETFHitDetailState::E_Chain_Juggle_Finish);
			}
		}
	}

	if (TFPlayerState->GetHitState() == ETFHitState::E_Guard)
	{
		switch (TFPlayerState->GetPlayerState())
		{
		case ETFPlayerState::E_Stand:
			{
				if (Attacker->TFPlayerState->GetAttackState() == ETFAttackState::E_UpperAttack)
				{
					TFPlayerState->SetHitDetailState(ETFHitDetailState::E_Guard_St);
				}

				if (Attacker->TFPlayerState->GetAttackState() == ETFAttackState::E_MiddleAttack)
				{
					TFPlayerState->SetHitDetailState(ETFHitDetailState::E_Guard_Mid);
				}
			}
			break;
		case ETFPlayerState::E_Crouch:
			TFPlayerState->SetHitDetailState(ETFHitDetailState::E_Guard_Cr);
			break;
		default:
			break;
		}
	}

	if (TFPlayerState->GetHitState() == ETFHitState::E_Juggle)
	{
		TFPlayerState->SetHitDetailState(ETFHitDetailState::E_Chain_Juggle_Start);

		unsigned long DamagedAnimNo = 0;
		if(Attacker->TFPlayerState->GetAttackState() == ETFAttackState::E_UpperAttack)
		{
			DamagedAnimNo =  CurrentSerialNo * 10000 + static_cast<uint8>(ETFHitDetailState::E_ChainDamaged_Upper);
		}
		else if(Attacker->TFPlayerState->GetAttackState() == ETFAttackState::E_MiddleAttack)
		{
			DamagedAnimNo =  CurrentSerialNo * 10000 + static_cast<uint8>(ETFHitDetailState::E_ChainDamaged_Middle);
		}
		else if(Attacker->TFPlayerState->GetAttackState() == ETFAttackState::E_LowerAttack)
		{
			DamagedAnimNo =  CurrentSerialNo * 10000 + static_cast<uint8>(ETFHitDetailState::E_ChainDamaged_Lower);
		}
		
		ActionInputComponent->ChangeAction(DamagedAnimNo);
	}

	if (TFPlayerState->GetHitState() == ETFHitState::E_Counter)
	{
	}

	if (TFPlayerState->GetCurrentHitState() != ETFHitState::E_Juggle || TFPlayerState->GetCurrentHitState() != ETFHitState::E_Catch)
	{
		TFPlayerState->SetCurrentHitState(TFPlayerState->GetHitState());
	}
}

void APlayerCharacter::GetDamageBackup(EGetDamagedType type, APlayerCharacter* AttackCaster,
                                       UHitBoxComponent* HitComponent)
{
	//UE_LOG(LogTemp,Warning,TEXT("Finish : %hhd"), FinishState);

	CheckBlockingWall();
	if (IsValid(AttackCaster))
	{
		float Damage = 0;
		if (type == EGetDamagedType::E_Melee)
		{
			const ActionDataTable* AttackData = AttackCaster->ActionInputComponent->GetCurrentAction()->GetData();
			AttackCaster->ActionInputComponent->SetAttackState(EAttackerState::E_Attack);

			AttackCaster->PushWeakBoxComponents[0]->SetCollisionResponseToChannel(
				ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Overlap);

			bool isGuard = ActionInputComponent->GuardCheck(AttackData->AttackState);
			if (isGuard)
			{
				Damage = AttackData->B_Damage;
				GetPush(type, AttackCaster, HitComponent, AttackData->BlockStun);
			}
			else
			{
				Damage = AttackData->Damage;
				GetPush(type, AttackCaster, HitComponent, AttackData->HitStun);
				if (CurrentState == EStateofDamageBox::E_Fall || CurrentState == EStateofDamageBox::E_Jump)
				{
					if (AttackData->DamageActionNo != 70)
					{
						if (!isGuard && HitterState == EHitterState::E_Idle)
						{
							ActionInputComponent->ChangeAction(CurrentSerialNo * 10000 + DAMAGED_CR_AIR_H_START);
						}
					}
					else
					{
						if (!bBlockWall)
						{
							if (!isGuard && HitterState == EHitterState::E_Idle)
							{
								ActionInputComponent->
									ChangeAction(CurrentSerialNo * 10000 + AttackData->DamageActionNo);
							}
						}
						else
						{
							if (!isGuard && HitterState == EHitterState::E_Idle)
							{
								ActionInputComponent->ChangeAction(CurrentSerialNo * 10000 + DAMAGED_WALL_HIT);
							}
						}
					}
				}
				else
				{
					if (AttackData->Upper_Atk == 1)
					{
						if (!isGuard && HitterState == EHitterState::E_Idle)
						{
							ActionInputComponent->ChangeAction(CurrentSerialNo * 10000 + AttackData->DamageActionNo);
						}
					}
					else
					{
						if (AttackData->DamageActionNo == 70)
						{
							if (!bBlockWall)
							{
								if (!isGuard && HitterState == EHitterState::E_Idle)
								{
									ActionInputComponent->ChangeAction(CurrentSerialNo * 10000 + DAMAGED_BLOWWALL);
								}
							}
							else
							{
								if (!isGuard && HitterState == EHitterState::E_Idle)
								{
									ActionInputComponent->ChangeAction(CurrentSerialNo * 10000 + DAMAGED_WALL_HIT);
								}
							}
						}
						else
						{
							if (!isGuard && HitterState == EHitterState::E_Idle)
							{
								ActionInputComponent->
									ChangeAction(CurrentSerialNo * 10000 + AttackData->DamageActionNo);
							}
						}
					}
				}
				if (AttackCaster->LauncherState != ELauncherState::E_Launcher && AttackData->Launcher_Atk == 1)
				{
					AttackCaster->LauncherState = ELauncherState::E_Launcher;
					AttackCaster->ActionInputComponent->SetLauncherSerialNo(
						AttackCaster->ActionInputComponent->GetCurrentAction()->GetData()->Chain_Action_Append);

					HitterState = EHitterState::E_Groggy;
				}
				else if (AttackCaster->LauncherState == ELauncherState::E_Launcher)
				{
					int32 JugglePoint = AttackCaster->ActionInputComponent->GetCurrentAction()->GetData()->Juggle;
					AttackCaster->TFPlayerState->AdditiveJuggleCount(JugglePoint);
					AttackCaster->TFPlayerState->AdditiveComboCount();
					if (AttackCaster->TFPlayerState->IsDisplayComboCount())
					{
						FString ComboString = FString::FromInt(AttackCaster->TFPlayerState->GetComboCount());
						// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red,
						//                                  FString::Printf(TEXT("Combo Count: %s"), *ComboString));
					}

					if (HitterState == EHitterState::E_Groggy || HitterState == EHitterState::E_GroggyContinue)
					{
						if (AttackCaster->ActionInputComponent->GetCurrentAction()->GetData()->Finish == 1)
						{
							if (AttackCaster->FinishState == EUseFinishState::E_Finish)
							{
								HitterState = EHitterState::E_GroggyFinish;
								AttackCaster->FinishState = EUseFinishState::E_None;
							}
						}
						else
						{
							if (AttackCaster->TFPlayerState->IsMaxJuggle())
							{
								HitterState = EHitterState::E_GroggyEnd;
							}
							else
							{
								HitterState = EHitterState::E_GroggyContinue;
							}
						}
					}
				}
			}
			if (!isGuard)
			{
				float HitInterval = AttackData->HitInterval / 1000.f;
				if (HitInterval > 0)
				{
					ActionInputComponent->StiffStop(AttackCaster, HitInterval, false);
					AttackCaster->ActionInputComponent->StiffStop(this, HitInterval, true);
				}
			}

			CurrentHp -= Damage;
			ShakeCameraAndParticlefromDamage(Damage, AttackCaster, HitComponent);
			CurrentSp = CurrentSp + AttackData->MP_Add_OH > MaxSp ? MaxSp : CurrentSp + AttackData->MP_Add_OH;
			AttackCaster->CurrentSp += AttackData->MP_Self_Add_OH > MaxSp ? MaxSp : AttackData->MP_Self_Add_OH;
		}
		else if (type == EGetDamagedType::E_Ranged)
		{
			const ActionDataTable* AttackData = AttackCaster->ActionInputComponent->GetActionDataByNum(
				AttackCaster->CurrentSerialNo * 10000 + 75)->GetData();
			bool isGuard = ActionInputComponent->GuardCheck(AttackData->AttackState);
			if (isGuard)
			{
				Damage = AttackData->B_Damage;
				GetPush(type, AttackCaster, HitComponent, AttackData->BlockStun);
			}
			else
			{
				Damage = AttackData->Damage;
				GetPush(type, AttackCaster, HitComponent, AttackData->HitStun);
				if (CurrentState == EStateofDamageBox::E_Fall || CurrentState == EStateofDamageBox::E_Jump)
				{
					if (AttackData->DamageActionNo != 70)
					{
						if (!isGuard && HitterState == EHitterState::E_Idle)
						{
							ActionInputComponent->ChangeAction(CurrentSerialNo * 10000 + DAMAGED_CR_AIR_H_START);
						}
					}
					else
					{
						if (!bBlockWall)
						{
							if (!isGuard && HitterState == EHitterState::E_Idle)
							{
								ActionInputComponent->
									ChangeAction(CurrentSerialNo * 10000 + AttackData->DamageActionNo);
							}
						}
						else
						{
							if (!isGuard && HitterState == EHitterState::E_Idle)
							{
								ActionInputComponent->ChangeAction(CurrentSerialNo * 10000 + DAMAGED_WALL_HIT);
							}
						}
					}
				}
				else
				{
					if (AttackData->Upper_Atk == 1)
					{
						if (!isGuard && HitterState == EHitterState::E_Idle)
						{
							ActionInputComponent->ChangeAction(CurrentSerialNo * 10000 + AttackData->DamageActionNo);
						}
					}
					else
					{
						if (AttackData->DamageActionNo == 70)
						{
							if (!bBlockWall)
							{
								if (!isGuard && HitterState == EHitterState::E_Idle)
								{
									ActionInputComponent->ChangeAction(CurrentSerialNo * 10000 + DAMAGED_BLOWWALL);
								}
							}
							else
							{
								if (!isGuard && HitterState == EHitterState::E_Idle)
								{
									ActionInputComponent->ChangeAction(CurrentSerialNo * 10000 + DAMAGED_WALL_HIT);
								}
							}
						}
						else
						{
							if (!isGuard && HitterState == EHitterState::E_Idle)
							{
								ActionInputComponent->
									ChangeAction(CurrentSerialNo * 10000 + AttackData->DamageActionNo);
							}
						}
					}
				}
			}

			if (!isGuard)
			{
				//Stiff();
				float HitInterval = AttackData->HitInterval / 1000.f;
				if (HitInterval > 0)
				{
					ActionInputComponent->StiffStop(AttackCaster, HitInterval, false);
				}
			}

			CurrentHp -= Damage;
			ShakeCameraAndParticlefromDamage(Damage, AttackCaster, HitComponent);
			CurrentSp = CurrentSp + AttackData->MP_Add_OH > MaxSp ? MaxSp : CurrentSp + AttackData->MP_Add_OH;
			AttackCaster->CurrentSp += AttackData->MP_Self_Add_OH > MaxSp ? MaxSp : AttackData->MP_Self_Add_OH;
		}

		LoseBattle(AttackCaster);
	}
}

void APlayerCharacter::GetPush(EGetDamagedType type, APlayerCharacter* PushCaster, UHitBoxComponent* HitComponent,
                               float PushValue)
{
	CheckBlockingWall();
	if (IsValid(PushCaster))
	{
		if (type != EGetDamagedType::E_Ranged)
		{
			if (bBlockWall)
			{
				float RotationValue;
				APlayerCharacter* PlayerCharacter = static_cast<APlayerCharacter*>(GetMesh()->GetOwner());
				if (PlayerCharacter)
				{
					RotationValue = ActionInputComponent->GetRotateValue();

					FVector currentPos = GetActorLocation();
					FVector enemyPos = PushCaster->GetActorLocation();

					FVector dir = enemyPos - currentPos;
					dir.Z = 0.0f;
					dir.Normalize();

					FVector forward = GetActionRotateValue() * -GetActorRightVector();
					forward.Z = 0.0f;
					forward.Normalize();

					double dot = FVector::DotProduct(dir, forward);
					if (dot > 0)
					{
						APlayerCharacter* OtherPlayer = Cast<APlayerCharacter>(PushCaster);
						if (OtherPlayer)
						{
							float AddForce = PushValue;
							//if (GetCharacterMovement()->IsFalling())
							//{
							//	AddForce = 3.0f;
							//}
							FVector PushDirection = FVector(0.f, dir.Y * AddForce, 0.f);

							OtherPlayer->AddActorWorldOffset(PushDirection, true);
						}
					}
				}
				return;
			}
		}

		if (!bBlockWall)
		{
			float RotationValue;
			APlayerCharacter* OtherPlayer = Cast<APlayerCharacter>(PushCaster);
			RotationValue = ActionInputComponent->GetRotateValue();

			FVector currentPos = GetActorLocation();
			FVector enemyPos = PushCaster->GetActorLocation();

			FVector dir = currentPos - enemyPos;
			dir.Z = 0.0f;
			dir.Normalize();

			FVector forward = OtherPlayer->GetActionRotateValue() * -OtherPlayer->GetActorRightVector();
			forward.Z = 0.0f;
			forward.Normalize();

			double dot = FVector::DotProduct(dir, forward);
			if (dot > 0)
			{
				float AddForce = PushValue;
				//if (GetCharacterMovement()->IsFalling())
				//{
				//	AddForce = 3.0f;
				//}
				FVector PushDirection = FVector(0.f, dir.Y * AddForce, 0.f);

				AddActorWorldOffset(PushDirection, true);
			}
		}
	}
}

void APlayerCharacter::LoseBattle(APlayerCharacter* Attacker)
{
	if (CurrentHp <= 0)
	{
		CurrentHp = 0;
	}

	if (CurrentHp == 0)
	{
		if (Attacker->ActionInputComponent->GetCurrentAction()->GetData()->CutScene != 0)
		{
			if (Attacker->ActionInputComponent->GetCurrentAction()->GetData()->CutScene == 1)
			{
				AInGameGameMode* IngameGameMode = Cast<AInGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
				if (IngameGameMode)
				{
					AGlobalCameraActor* GlobalCameraActor = Cast<AGlobalCameraActor>(IngameGameMode->GlobalCamera);
					if (GlobalCameraActor != nullptr)
					{
						GlobalCameraActor->ChangeFOV();
					}
				}
			}

			if (Attacker->ActionInputComponent->GetCurrentAction()->GetData()->CutScene == 2)
			{
				APlayerController* PlayerController = Cast<APlayerController>(
					UGameplayStatics::GetPlayerController(GetWorld(), 0));
				if (PlayerController)
				{
					PlayerController->SetViewTargetWithBlend(Attacker->SkillCamera->GetChildActor(), 0.2f);
				}

				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
				{
					AInGameGameMode* IngameGameMode = Cast<AInGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
					if (IngameGameMode)
					{
						if (IngameGameMode->GlobalCamera != nullptr)
						{
							APlayerController* PlayerController = Cast<APlayerController>(
								UGameplayStatics::GetPlayerController(GetWorld(), 0));
							if (PlayerController)
							{
								PlayerController->SetViewTargetWithBlend(IngameGameMode->GlobalCamera,
								                                         0.5f);
							}
						}
					}
				}), 1.0f, false);
			}
		}

		ActionInputComponent->EndCombo();
		//ActionInputComponent->ChangeAction(CurrentSerialNo * 10000 + KO_H);

		if (Fuc_BattleLose.IsBound())
			Fuc_BattleLose.Execute();
	}
}

void APlayerCharacter::WinBattle()
{
	ActionInputComponent->ChangeAction(CurrentSerialNo * 10000 + RESULT_WIN);
}

void APlayerCharacter::AddGp(float value)
{
}

void APlayerCharacter::RemoveGp(float value)
{
}

void APlayerCharacter::DecreaseGp(float value)
{
	bUseGuard = true;
	DecreseGpValue = value;
}

float APlayerCharacter::GetActionRotateValue()
{
	return ActionInputComponent->GetRotateValue();
}

void APlayerCharacter::CutSceneProcess(bool isPause)
{
	APlayerController* PlayerController = Cast<APlayerController>(
		UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController)
	{
		PlayerController->SetViewTargetWithBlend(CutSceneCamera->GetChildActor(),
		                                         CombatComponent->fCutSceneCameraBlendTime);

		if (isPause)
		{
			Darkness(true);
		}
		else
		{
			AInGameGameMode* GameMode = Cast<AInGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

			if (GameMode)
			{
				ESlateVisibility Visibility = ESlateVisibility::Hidden;

				GameMode->UpperPannelOnOff(Visibility);
			}
		}
	}
}

float APlayerCharacter::GetAnimIntroTime(unsigned long Index)
{
	return ActionInputComponent->GetActionMontageTime(Index);
}

void APlayerCharacter::Darkness(bool isUse)
{
	AInGameGameMode* GameMode = Cast<AInGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GameMode)
	{
		ESlateVisibility Visibility = ESlateVisibility::Hidden;
		if (isUse)
		{
			Visibility = ESlateVisibility::Hidden;
			UsingSkillTime(Dilationtime);
		}
		else
		{
			Visibility = ESlateVisibility::Visible;
			UsingSkillTime(1);
		}
		GameMode->UpperPannelOnOff(Visibility);
		GameMode->DarknessFreezingController(isUse);
	}
}

void APlayerCharacter::UsingSkillTime(float time)
{
	GetWorldSettings()->SetTimeDilation(time);
	CurrentCharacterTime = 1 / time;
	bChangeTime = true;
}

void APlayerCharacter::JumpStart(EJumpState jumpState)
{
	ActionInputComponent->JumpStart(jumpState);
}

void APlayerCharacter::JumpAttackCheck()
{
	ActionInputComponent->JumpAttackCheck();
}

void APlayerCharacter::ActivateJumpAttack()
{
	ActionInputComponent->ActivateJumpAttack();
}

void APlayerCharacter::PushEnemy()
{
	float RotationValue;
	APlayerCharacter* PlayerCharacter = static_cast<APlayerCharacter*>(GetMesh()->GetOwner());
	if (PlayerCharacter)
	{
		RotationValue = PlayerCharacter->ActionInputComponent->GetRotateValue();

		FVector currentPos = PlayerCharacter->GetActorLocation();
		FVector enemyPos = m_pEnemy->GetActorLocation();

		FVector dir = enemyPos - currentPos;
		dir.Z = 0.0f;
		dir.Normalize();

		FVector forward = PlayerCharacter->GetActionRotateValue() * -PlayerCharacter->GetActorRightVector();
		forward.Z = 0.0f;
		forward.Normalize();

		double dot = FVector::DotProduct(dir, forward);
		if (dot > 0)
		{
			APlayerCharacter* OtherPlayer = Cast<APlayerCharacter>(m_pEnemy);
			if (OtherPlayer)
			{
				float AddForce = 1.5f;
				if (PlayerCharacter->GetCharacterMovement()->IsFalling())
				{
					AddForce = 3.0f;
				}
				FVector PushDirection = FVector(0.f, -RotationValue * AddForce, 0.f);

				OtherPlayer->AddActorWorldOffset(PushDirection, true);
			}
		}
	}
}

void APlayerCharacter::OnPlayerHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
}

void APlayerCharacter::OnPlayerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                            const FHitResult& SweepResult)
{
	m_pEnemy = OtherActor;
	bPush = true;
}

void APlayerCharacter::OnPlayerOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	m_pEnemy = nullptr;
	bPush = false;
}
