// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GlobalCameraActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/TokenFighterGameInstance.h"

// Sets default values
AGlobalCameraActor::AGlobalCameraActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneRoot);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->AttachToComponent(SceneRoot, FAttachmentTransformRules::KeepRelativeTransform);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AGlobalCameraActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGlobalCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGlobalCameraActor::SetPlayers()
{
	Player01 = Cast<UTokenFighterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->Player01;
	Player02 = Cast<UTokenFighterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->Player02;
}

void AGlobalCameraActor::ChangeFOV()
{
	ChangeFOVIn();
}

void AGlobalCameraActor::ChangeFOVIn()
{
	GetWorld()->GetTimerManager().SetTimer(FOVTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		ElapsedTime += GetWorld()->GetTimerManager().GetTimerElapsed(FOVTimerHandle);

		float Alpha = FMath::Clamp(ElapsedTime / FirstLerpTime, 0.0f, 1.0f);

		float NewFOV = FMath::Lerp(DefaultFOV, EndFOV, Alpha);

		CameraComp->SetFieldOfView(NewFOV);

		//UE_LOG(LogTemp,Warning,TEXT("NewFov: %f"), NewFOV);
		
		if (Alpha >= 1.0f)
		{
			ElapsedTime = 0.f;
			GetWorldTimerManager().ClearTimer(FOVTimerHandle);
			ChangeFOVOut();
		}
	}), 0.01f, true);
}

void AGlobalCameraActor::ChangeFOVOut()
{
	GetWorld()->GetTimerManager().SetTimer(FOVTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		ElapsedTime += GetWorld()->GetTimerManager().GetTimerElapsed(FOVTimerHandle);

		float Alpha = FMath::Clamp(ElapsedTime / LerpTime, 0.0f, 1.0f);

		float NewFOV  = FMath::Lerp(EndFOV, DefaultFOV, Alpha);
		
		CameraComp->SetFieldOfView(NewFOV);

		//UE_LOG(LogTemp,Warning,TEXT("NewFov: %f"), NewFOV);

		if (Alpha >= 1.0f)
		{
			ElapsedTime = 0.f;
			GetWorldTimerManager().ClearTimer(FOVTimerHandle);
		}
	}), 0.01f, true);
}

