// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TFPlayerState.generated.h"

UENUM(BlueprintType)
enum class ETFPlayerState : uint8
{
	E_Stand = 0 UMETA(DisplayName = "Stand"),
	E_Crouch = 1 UMETA(DisplayName = "Crouch"),
	E_Jump = 2 UMETA(DisplayName = "Jump"),
	E_Down = 3 UMETA(DisplayName = "Down"),
};

UENUM(BlueprintType)
enum class ETFAttackState : uint8
{
	E_Idle = 0 UMETA(DisplayName = "Idle"), 
	E_UpperAttack = 1 UMETA(DisplayName = "UpperAttack"),
	E_MiddleAttack = 2 UMETA(DisplayName = "MiddleAttack"), 
	E_LowerAttack = 3 UMETA(DisplayName = "LowerAttack"), 
};

UENUM(BlueprintType)
enum class ETFAttackDetailState : uint8
{
	E_Idle = 0 UMETA(DisplayName = "Idle"),
	E_Launcher = 4 UMETA(DisplayName = "Launcher"),
	E_Throw = 5 UMETA(DisplayName = "Throw"),
	E_UpperLauncher = 1 UMETA(DisplayName = "UpperLauncher"),
	E_MiddleLauncher = 2 UMETA(DisplayName = "MiddleLauncher"),
	E_LowerLauncher = 3 UMETA(DisplayName = "LowerLauncher"),
};

UENUM(BlueprintType)
enum class ETFHitState : uint8
{
	E_Idle = 0 UMETA(DisplayName = "Idle"), 
	E_Hit =  1 UMETA(DisplayName = "Hit"),
	E_Guard =  2 UMETA(DisplayName = "Guard"),
	E_Juggle =  3 UMETA(DisplayName = "Juggle"),
	E_Invincibility =  4 UMETA(DisplayName = "Invincibility"),
	E_Counter  =  5 UMETA(DisplayName = "Counter"),
	E_Catch = 6 UMETA(DisplayName = "Catch"),
};

UENUM(BlueprintType)
enum class ETFHitDetailState : uint8
{
	E_Idle = 0 UMETA(DisplayName = "Idle"),
	E_St_Up_Weak_Hit_1 = 101 UMETA(DisplayName = "St_Up_Weak_Hit_1"), //rand _1 ~ _3
	E_St_Up_Weak_Hit_2 = 102 UMETA(DisplayName = "St_Up_Weak_Hit_2"),
	E_St_Up_Weak_Hit_3 = 103 UMETA(DisplayName = "St_Up_Weak_Hit_3"),
	E_St_Up_Strong_Hit_1 = 104  UMETA(DisplayName = "St_Up_Strong_Hit_1"), //rand _1 ~ _3
	E_St_Up_Strong_Hit_2 = 105 UMETA(DisplayName = "St_Up_Strong_Hit_2"),
	E_St_Up_Strong_Hit_3 = 106 UMETA(DisplayName = "St_Up_Strong_Hit_3"),
	E_St_Mid_Hit = 107 UMETA(DisplayName = "St_Mid_Hit"),
	E_St_Low_Weak_Hit = 108 UMETA(DisplayName = "St_Low_Weak_Hit"),
	E_St_Low_Strong_Hit = 109  UMETA(DisplayName = "St_Low_Strong_Hit"),
	E_St_Down_Front = 110 UMETA(DisplayName = "St_Down_Front"),
	E_St_Down_Back = 111 UMETA(DisplayName = "St_Down_Back"),
	E_St_Down_Critical = 112 UMETA(DisplayName = "St_Down_Critical"),
	E_Cr_Up_Weak_Hit_1 = 113 UMETA(DisplayName = "Cr_Up_Weak_Hit_1"), //rand _1 ~ _3
	E_Cr_Up_Weak_Hit_2 = 114 UMETA(DisplayName = "Cr_Up_Weak_Hit_2"), 
	E_Cr_Up_Weak_Hit_3 = 115 UMETA(DisplayName = "Cr_Up_Weak_Hit_3"), 
	E_Cr_Up_Strong_Hit_1 = 116 UMETA(DisplayName = "Cr_Up_Strong_Hit"), //rand _1 ~ _3
	E_Cr_Up_Strong_Hit_2 = 117 UMETA(DisplayName = "Cr_Up_Strong_Hit"), 
	E_Cr_Up_Strong_Hit_3 = 118 UMETA(DisplayName = "Cr_Up_Strong_Hit"), 
	E_Cr_Mid_Hit = 119 UMETA(DisplayName = "Cr_Mid_Hit"),
	E_Cr_Low_Weak_Hit = 120 UMETA(DisplayName = "Cr_Low_Weak_Hit"),
	E_Cr_Low_Strong_Hit = 121 UMETA(DisplayName = "Cr_Low_Strong_Hit"),
	E_Cr_Down_Front = 122 UMETA(DisplayName = "Cr_Down_Front"),
	E_Cr_Down_Back = 123 UMETA(DisplayName = "Cr_Down_Back"),
	E_Cr_Down_Critical = 124 UMETA(DisplayName = "Cr_Down_Critical"),
	E_Jump_Weak_Hit = 125 UMETA(DisplayName = "Jump_Weak_Hit"),
	E_Jump_Strong_Hit = 126 UMETA(DisplayName = "Jump_Strong_Hit"),
	E_Guard_St = 127 UMETA(DisplayName = "Guard_St"),
	E_Guard_Mid = 128 UMETA(DisplayName = "Guard_Mid"),
	E_Guard_Cr = 129 UMETA(DisplayName = "Guard_Cr"),
	E_ChainDamaged_Upper = 210 UMETA(DisplayName = "ChainDamaged_Upper"),
	E_ChainDamaged_Middle = 211 UMETA(DisplayName = "ChainDamaged_Middle"),
	E_ChainDamaged_Lower = 212 UMETA(DisplayName = "ChainDamaged_Lower"),
	E_Chain_Juggle_Start = 214 UMETA(DisplayName = "Chain_Juggle_Start"),
	E_Chain_Juggle_Hit = 215 UMETA(DisplayName = "Chain_Juggle_Hit"),
	E_Chain_Juggle_Finish = 216 UMETA(DisplayName = "Chain_Juggle_Finish"),
	E_Throw_Start = 224 UMETA(DisplayName = "Throw_Start"),
	E_Throw_Success_Hit = 225 UMETA(DisplayName = "Throw_Success_Hit"),
	E_Throw_Fail_Tech = 226 UMETA(DisplayName = "Throw_Fail_Tech"),
};

UENUM(BlueprintType)
enum class ETFEnvironmentState : uint8
{
	E_Idle = 0 UMETA(DisplayName = "Idle"),
	E_Wall = 1 UMETA(DisplayName = "Wall"),
};

UCLASS()
class PROJECTTF_API ATFPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ATFPlayerState();
	
	void SetPlayerState(ETFPlayerState PlayerState){PS = PlayerState;}
	ETFPlayerState GetPlayerState() const {return PS;}

	void SetAttackState(ETFAttackState AttackerState){AS = AttackerState;}
	ETFAttackState GetAttackState() const {return AS;}

	void SetHitState(ETFHitState HitState){HS = HitState;}
	ETFHitState GetHitState() const {return HS;}
	
	void SetAttackDetailState(ETFAttackDetailState AttackDetailState){ADS = AttackDetailState;}
	ETFAttackDetailState GetAttackDetailState() const {return ADS;}

	void SetHitDetailState(ETFHitDetailState HitDetailState){HDS = HitDetailState;}
	ETFHitDetailState GetHitDetailState() const {return HDS;}
	
	void SetEnvironmentState(ETFEnvironmentState EnvironmentState){ES = EnvironmentState;}
	ETFEnvironmentState GetEnvironmentState() const {return ES;}

	void SetCurrentHitState(ETFHitState HitState){CurrentHitState = HitState;}
	ETFHitState GetCurrentHitState() const {return CurrentHitState;}

	void SetMaxJuggleCount(float Count){MaxJuggleCount = Count;}

	void AdditiveJuggleCount(int32 Count);

	void ResetJuggleCount(){JuggleCount = 0;}

	bool IsMaxJuggle() const;

	void AdditiveComboCount(){++ComboCount;}

	void ResetComboCount(){ComboCount = 0;}

	bool IsDisplayComboCount() const;

	int8 GetComboCount() const {return ComboCount;}

private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	ETFPlayerState PS;
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	ETFAttackState AS;
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	ETFHitState HS;
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	ETFAttackDetailState ADS;
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	ETFHitDetailState HDS;
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	ETFEnvironmentState ES;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	ETFHitState CurrentHitState;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	int8 JuggleCount;
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	int8 MaxJuggleCount;
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	int8 ComboCount;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	float HP;
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	float MaxHP;
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	float SP;
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	float MaxSP;
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	float GP;
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	float MaxGP;
};
