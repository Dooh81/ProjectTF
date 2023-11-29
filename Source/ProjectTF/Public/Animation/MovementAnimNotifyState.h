// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/PlayerCharacter.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Core/DataTable.h"
#include "MovementAnimNotifyState.generated.h"


using namespace DataTable;
/**
 * 
 */


UCLASS()
class PROJECTTF_API UMovementAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	UNiagaraSystem* Dash_FX;

protected:
	UPROPERTY(EditAnywhere)
	float MoveSpeed;

	CharacterDataTable* m_pCharacterDataTable;
private:
	APlayerCharacter* Owner;
};
