// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Core/ChainDamagedActionData.h"
#include "ChainDamagedAnimNotifyState.generated.h"

/**
 * 
 */


UCLASS()
class PROJECTTF_API UChainDamagedAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	class APlayerCharacter* m_pOwner;

	UPROPERTY(EditAnywhere)
	EChainDamagedType ChainDamagedType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combo", meta = (ToolTip = "Y: Forward Distance, Z: UpperDistance"))
	FVector HitChainDistance;
	
	bool bOnes =false;
};
