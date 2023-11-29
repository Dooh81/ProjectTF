// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CombatComponent.h"
#include "Core/ChainDamagedActionData.h"
#include "Kismet/GameplayStatics.h"
#include "Core/GlobalCameraActor.h"
#include "LegacyCameraShake.h"


// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UCombatComponent::GetLauncherGravityScale(EChainDamagedType Type)
{
	float returnGravityScale = 0.0f;

	switch (Type)
	{
	case EChainDamagedType::E_None:
		UE_LOG(LogTemp, Error, TEXT("EChainDamagedType is None"));
		break;
	case EChainDamagedType::E_Lower:
		returnGravityScale = fLowerLauncherGravityScale;
		break;
	case EChainDamagedType::E_Middle:
		returnGravityScale = fMiddleLauncherGravityScale;
		break;
	case EChainDamagedType::E_Upper:
		returnGravityScale = fUpperLauncherGravityScale;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("EChainDamagedType is not Valid"))
		break;
	}

	return returnGravityScale;
}

FVector UCombatComponent::GetLauncherDistance(EChainDamagedType Type)
{
	FVector ReturnDistance = FVector::ZeroVector;

	switch (Type)
	{
	case EChainDamagedType::E_None:
		UE_LOG(LogTemp, Error, TEXT("EChainDamagedType is None"));
		break;
	case EChainDamagedType::E_Lower:
		ReturnDistance = LowerLauncherDistance;
		break;
	case EChainDamagedType::E_Middle:
		ReturnDistance = MiddleLauncherDistance;
		break;
	case EChainDamagedType::E_Upper:
		ReturnDistance = UpperLauncherDistance;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("EChainDamagedType is not Valid"))
		break;
	}

	return ReturnDistance;
}

void UCombatComponent::CameraShake()
{
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGlobalCameraActor::StaticClass(), Camera);
	if (IsValid(Camera[0]))
	{
		if (IsValid(CameraShakeSource))
		{
			CameraShakeSource.GetDefaultObject()->LocOscillation.Y.Amplitude = 15.0f;
			CameraShakeSource.GetDefaultObject()->LocOscillation.Y.Frequency = 100.0f;
			CameraShakeSource.GetDefaultObject()->LocOscillation.Z.Amplitude = 15.0f;
			CameraShakeSource.GetDefaultObject()->LocOscillation.Z.Frequency = 100.0f;
			CameraShakeSource.GetDefaultObject()->FOVOscillation.Amplitude = 15.0f;
			CameraShakeSource.GetDefaultObject()->FOVOscillation.Frequency = 100.0f;
			UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShakeSource, Camera[0]->GetActorLocation(), 0, 1000);

		}
	}

}
