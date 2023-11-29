// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "JumpStartNotify.generated.h"

class APlayerCharacter;
UENUM(BlueprintType)
enum class EJumpState : uint8
{
	E_Vertical = 0 UMETA(DisplayName = "Vertical"),
	E_Forward UMETA(DisplayName = "Forward"),
	E_Back UMETA(DisplayName = "Back"),
};

/**
 * 
 */
UCLASS()
class PROJECTTF_API UJumpStartNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EJumpState JumpState;

	APlayerCharacter* Owner;
	
};
