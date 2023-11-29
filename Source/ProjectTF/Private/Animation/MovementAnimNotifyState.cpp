// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/MovementAnimNotifyState.h"

#include "Core/ActionInputComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Core/DataTableManager.h"

#define MIN 0.f
#define MAX 1000.f

void UMovementAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	Owner = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (Owner != nullptr)
	{
		m_pCharacterDataTable = (DataTable::CharacterDataTable*)DataTableManager::GetInstance()->GetDataTable("Character", Owner->GetSerialNo());
		
		float FowardDash = m_pCharacterDataTable->Foward_Dash / MAX;
		float Clamp = FMath::Clamp(FowardDash, MIN, MAX);
		//MoveSpeed = Clamp;

		
		Dash_FX = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/Fighting_template/Effect/Token_Effect/Particles/VFX_EV_dust01.VFX_EV_dust01"));

		FVector SpawnLocation = Owner->GetActorLocation();
		SpawnLocation.Z = 0.0f;

		FRotator SpawnRotator = FRotator(0.0f, 0.0f, 0.0f);
		if(Owner->ActionInputComponent->GetRotateValue() < 0.0f)
		{
			SpawnRotator.Yaw = 180.0f;
		}
		
		//UNiagaraFunctionLibrary ::SpawnSystemAttached(Dash_FX,Owner->GetMesh(),FName(""),FVector::Zero(),FRotator(0.0f,180.0f,0.0f),EAttachLocation::KeepRelativeOffset,true,true);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(MeshComp->GetWorld(),Dash_FX,SpawnLocation,SpawnRotator,FVector::OneVector,false,true);
		
	}
}

void UMovementAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if(Owner !=nullptr)
	{
		FVector forward = Owner->GetActionRotateValue() * -Owner->GetActorRightVector();
		forward.Z = 0.0f;
		forward.Normalize();
		Owner->MoveValue = MoveSpeed;
		Owner->AddMovementInput(forward, MoveSpeed);
	}
}

void UMovementAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	
}
