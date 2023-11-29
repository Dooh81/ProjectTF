// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/DebugAnimNotify.h"

void UDebugAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	FString DebugMessage = "Debugging AnimName : " + Animation->GetName();
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, DebugMessage);
}
