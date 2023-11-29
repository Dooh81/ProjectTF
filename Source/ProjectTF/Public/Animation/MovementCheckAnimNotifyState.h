// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/PlayerCharacter.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "MovementCheckAnimNotifyState.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTTF_API UMovementCheckAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UMovementCheckAnimNotifyState();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	//초당 이동거리
	UPROPERTY(EditAnywhere)
	float MoveDistancePerSec;

private:

	APlayerCharacter* Owner;

	//전체이동거리
	float TotalDistance;

	//전체 시간
	float TotalTime;

	//시작 위치
	FVector StartLocation;

	FVector Direction;

	//이동 타이머
	FTimerHandle MoveTimerHandle;
};
