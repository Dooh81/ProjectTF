// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/EngineTypes.h"
#include "Animation/JumpStartNotify.h"
#include "GameFramework/TFPlayerState.h"
#include "PlayerCharacter.generated.h"

class ActionData;

UENUM(BlueprintType)
enum class EPartsOfHitBox : uint8
{
	E_None = 0 UMETA(DisplayName = "None"),
	E_Head = 1 UMETA(DisplayName = "Head"), 
	E_Arm_L =2 UMETA(DisplayName = "Arm_L"),
	E_Arm_R =3 UMETA(DisplayName = "Arm_R"),
	E_Leg_L =4 UMETA(DisplayName = "Leg_L"),
	E_Leg_R =5 UMETA(DisplayName = "Leg_R"),
	E_Front_High UMETA(DisplayName = "Front_High"),
	E_Front_Mid UMETA(DisplayName = "Front_Mid"),
	E_Front_Low UMETA(DisplayName = "Front_Low"),
	E_Upper UMETA(DisplayName = "Upper"),
	E_FlashKick UMETA(DisplayName = "FlashKick"),
};

UENUM(BlueprintType)
enum class EUseFinishState : uint8
{
	E_None = 0 UMETA(DisplayName = "None"),
	E_Finish = 1 UMETA(DisplayName = "Finish"), 
};

UENUM(BlueprintType)
enum class EStateofDamageBox : uint8
{
	E_Invincible = 0 UMETA(DisplayName = "Invincible"),
	E_Idle = 1 UMETA(DisplayName = "Idle"), 
	E_Crouch = 2 UMETA(DisplayName = "Crouch"),
	E_Down = 3 UMETA(DisplayName = "Down"),
	E_Jump = 4 UMETA(DisplayName = "Jump"),
	E_Fall = 5 UMETA(DisplayName = "Fall"),
	E_Wall = 6 UMETA(DisplayName = "Wall"),
};

UENUM(BlueprintType)
enum class EAttackerState : uint8
{
	E_Idle = 0 UMETA(DisplayName = "Idle"), 
	E_Attack = 1 UMETA(DisplayName = "Attack"),
};

UENUM(BlueprintType)
enum class EHitterState : uint8
{
	E_Idle = 0 UMETA(DisplayName = "Idle"), 
	E_Groggy = 1 UMETA(DisplayName = "Groggy"),
	E_GroggyContinue = 2 UMETA(DisplayName = "GroggyContinue"),
	E_GroggyFinish = 3 UMETA(DisplayName = "GroggyFinish"),
	E_GroggyEnd = 4 UMETA(DisplayName = "GroggyEnd")
};

UENUM(BlueprintType)
enum class ELauncherState : uint8
{
	E_Idle = 0 UMETA(DisplayName = "Idle"), 
	E_Launcher = 1 UMETA(DisplayName = "Launcher"),
	E_UpperLauncher = 2 UMETA(DisplayName = "UpperLauncher"),
	E_MiddleLauncher = 3 UMETA(DisplayName = "MiddleLauncher"),
	E_LowerLauncher = 4 UMETA(DisplayName = "LowerLauncher"),
};

UENUM(BlueprintType)
enum class EChainDamagedType : uint8
{
	E_None = 0 UMETA(DisplayName = "None"),
	E_Upper = 1 UMETA(DisplayName = "Upper"), 
	E_Middle =2 UMETA(DisplayName = "Middle"),
	E_Lower =3 UMETA(DisplayName = "Lower"),
};

UENUM(BlueprintType)
enum class EGetDamagedType : uint8
{
	E_None = 0 UMETA(DisplayName = "None"),
	E_Melee = 1 UMETA(DisplayName = "Melee"),
	E_Ranged = 2 UMETA(DisplayName = "Ranged"),
	E_Self = 3 UMETA(DisplayName = "Self"),
	E_DOT = 4 UMETA(DisplayName = "DOT"),
};

DECLARE_DELEGATE(FDele_BattleLose);

UCLASS()
class PROJECTTF_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Dilationtime = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentCharacterTime = 1.0f;

	bool bChangeTime;

	UFUNCTION(BlueprintCallable)
	void SetupCapsuleCollision();

	UFUNCTION(BlueprintCallable)
	void BaseStateInitailize();

	UFUNCTION(BlueprintCallable)
	void CharacterIntialize();

	UFUNCTION()
	void OnPlayerHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnPlayerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
	void PushEnemy();

	void JumpStart(EJumpState jumpState);

	void JumpAttackCheck();

	void ActivateJumpAttack();

	FString GetAnimLocalPath() { return AnimLocalPath; }

	void AddSP(ActionData* pActionData);

	ATFPlayerState* TFPlayerState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* PlayerCapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UActionInputComponent* ActionInputComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class UHitBoxComponent*> HitBoxComponents;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class UHitBoxComponent*> DamageBoxComponents;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class UHitBoxComponent*> ThrowHitBoxComponents;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class UHitBoxComponent*> ThrowDamageBoxComponents;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class UHitBoxComponent*> PushWeakBoxComponents;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class UHitBoxComponent*> PushStrongBoxComponents;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UHitBoxComponent* ActiveHitBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UHitBoxComponent* ActiveDamageBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UCombatComponent* CombatComponent;

	EStateofDamageBox CurrentState;

	ELauncherState LauncherState;
	
	EAttackerState AttackerState;

	EHitterState HitterState;

	EUseFinishState FinishState;

	bool bUseFinish;

	UFUNCTION(Blueprintpure, Category = CharacterState)
		ELauncherState GetLauncherState() { return LauncherState; }

	UFUNCTION(Blueprintpure, Category = CharacterState)
		EAttackerState GetAttackerState() { return AttackerState; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class UChildActorComponent* CutSceneCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UChildActorComponent* SkillCamera;
	
	//temp
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Jump)
	float TestJumpHeight = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Jump)
	float TestJumpForward =700.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Jump)
	float TestJumpBackward = 700.f;

	UFUNCTION()
	void ActiveHitBoxByIndex(TArray<struct FBoxStatus> BoxStatus, EUseFinishState finish);
	UFUNCTION()
	void ChangePushBox(struct FBoxStatus BoxStatus);

	UFUNCTION()
	void ActivePushBox(bool bActive);


	UFUNCTION()
	void ActiveFinishState(EUseFinishState finish);

	UFUNCTION()
	void ChangeHitBoxTransform(TArray<struct FBoxStatus> BoxStatus);

	UFUNCTION()
	void ChangeDamageBoxTransform(EStateofDamageBox state,TArray<struct FBoxStatus> BoxStatus);

	UFUNCTION()
	void DeActiveHitBox();
	
	UFUNCTION()
	void ActiveDamegeBox(EStateofDamageBox state);

	UFUNCTION()
		void MatchPushBoxtoCollision();

	UFUNCTION(BlueprintCallable)
	void StartActionCheck();
	UFUNCTION(BlueprintCallable)
	void EndActionCheck();
	UFUNCTION(BlueprintCallable)
	void EndActionAnimation();

	UFUNCTION(BlueprintCallable)
	void SettingCutScenePlayerCamera(FPostProcessSettings PostProcessSettigs);

	UPROPERTY(EditDefaultsOnly,BlueprintReadwrite)
		FString AnimLocalPath;

	long GetSerialNo();
	
	UFUNCTION(BlueprintCallable)
		void GetDamage(EGetDamagedType type, APlayerCharacter* AttackCaster, UHitBoxComponent* HitComponent);

	UFUNCTION(BlueprintCallable)
	void DamagedActionPlay();
	
	UFUNCTION(BlueprintCallable)
		void GetThrow(EGetDamagedType type, APlayerCharacter* AttackCaster, UHitBoxComponent* HitComponent);

	void GetDamageMakeHitState(APlayerCharacter* Attacker);
	
	UFUNCTION(BlueprintCallable)
	void GetDamageBackup(EGetDamagedType type, APlayerCharacter* AttackCaster, UHitBoxComponent* HitComponent);

	
	UFUNCTION(BlueprintCallable)
		void GetPush(EGetDamagedType type, APlayerCharacter* PushCaster, UHitBoxComponent* HitComponent,float PushPower);

	//승패시 캐릭터 액션
	UFUNCTION(BlueprintCallable)
		void LoseBattle(APlayerCharacter* Attacker);
	UFUNCTION(BlueprintCallable)
		void WinBattle();

	//액션으로 소비되고 공급되는 Gp
	UFUNCTION(BlueprintCallable)
		void AddGp(float value);
	UFUNCTION(BlueprintCallable)
		void RemoveGp(float value);
	//가드키를 누를떄 소비되는 Gp
	UFUNCTION(BlueprintCallable)
		void DecreaseGp(float value);
	//사용하는지 체크하고
	bool bUseGuard;
	//줄어드는 수치
	float DecreseGpValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMirror = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float fMirrorBlendTime = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
		bool bCanControl;

	UFUNCTION(Blueprintpure)
	float GetActionRotateValue();

	void CutSceneProcess(bool isPause);

	float GetAnimIntroTime(unsigned long Index);

	void Darkness(bool isUse);

	UFUNCTION(BlueprintCallable)
		void UsingSkillTime(float time);
	
	UFUNCTION(BlueprintCallable)
	void ChangeCameraBattle();

	UFUNCTION(BlueprintCallable)
	void ChangeActionIntro(FString SelectNumber);

	UFUNCTION(BlueprintImplementableEvent, Category = "BlueprintCallEvent")
    void ShakeCameraAndParticlefromDamage(float Damage, APlayerCharacter* AttackCaster, UHitBoxComponent* HitComponent);

	UFUNCTION(BlueprintCallable)
	void DamageCheck(UObject* OtherActor, UPrimitiveComponent* OtherComponent);
	UFUNCTION(BlueprintCallable)
	void PushCheck(UObject* OtherActor, UPrimitiveComponent* OtherComponent);
	UFUNCTION(BlueprintCallable)
	void ThrowCheck(UObject* OtherActor, UPrimitiveComponent* OtherComponent);
	UFUNCTION(BlueprintCallable)
	void EndPush();

	bool bEndPush = true;
private:
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* WeakAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* StrongAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ParryAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* SpecialAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InputRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InputLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InputUp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InputThrow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InputCameraChangeBattle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InputCameraChangeThirdPerson;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InputMayCIntro;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InputBayCIntro;
	
	bool bDoOnce1 = false;
	bool bDoOnce2 = false;
	bool bDoOnce3 = false;
	bool bPush = false;
public:
	bool bOverlappedHitBox;
	EGetDamagedType HitDamagedType;
	APlayerCharacter* HitPlayer;
	UHitBoxComponent* HitPlayerHitBox;

	bool bOverlappedThrowBox;

	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		bool bOverlappedPushBox;
	EGetDamagedType PushType;
	APlayerCharacter* PushPlayer;
	UHitBoxComponent* PushPlayerHitBox;
	UPROPERTY(EditAnywhere)
	float PushPower = 1000.0f;

	AActor* m_pEnemy;

public:
	void UpInput();
	void ResetUpInput();
	void DownInput();
	void ResetDownInput();
	void RightInput();
	void ResetRightInput();
	void LeftInput();
	void ResetLeftInput();
	void WeakInput();
	void ResetWeakInput();
	void StrongInput();
	void ResetStrongInput();
	void JumpInput();
	void ResetJumpInput();
	void ParryInput();
	void ResetParryInput();
	void SpecialInput();
	void ResetSpecialInput();
	void ThrowInput();
	void ResetThrowInput();
	void ResetInput();
	void CheckBlockingWall();
	void Stiff();
	void HitStop();
	void GuardStop();
	void EnemyEndCombo();
	void PlayIntro();
	bool EndGameMoving();

	void MirrorChangeBlendMaterial();

public:
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Character, meta = (AllowPrivateAccess = "true"))
		class APawn* VSCharacter;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
		bool bBeginAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentHp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaxHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentSp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaxSp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentGp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaxGp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int64 CurrentSerialNo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 ColIndex;
	FDele_BattleLose Fuc_BattleLose;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bBlockWall;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bKnockDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UNiagaraSystem* AdogenSystem_H;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UNiagaraSystem* AdogenSystem_L;

	UPROPERTY(VisibleAnywhere)
		float MoveValue;

	TArray<UMaterialInstanceDynamic*> DynamicMirrorMaterials;

	int InitMirror() { return 1 - bMirror * 2; }

	TArray<TEnumAsByte<EObjectTypeQuery>> WallBlockObjectTypes; // 히트 가능한 오브젝트 유형들.
};
