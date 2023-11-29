// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/HitBoxComponent.h"
#include "Core/PlayerCharacter.h"
#include "Core/WallActor.h"
#include "GameFramework/InGameGameMode.h"
#include "Kismet/GameplayStatics.h"

UHitBoxComponent::UHitBoxComponent()
{
	InitByHitBoxType(EHitBoxType::E_Hit);
	LineThickness = 1.0f;
}

void UHitBoxComponent::BeginPlay()
{
	Super::BeginPlay();
	InitByHitBoxType(HitBoxType);
	if (HitBoxType == EHitBoxType::E_Damage)
	{
		OnComponentBeginOverlap.AddDynamic(this, &UHitBoxComponent::OverlapDamageCheck);
		//OnComponentHit.AddDynamic(this, &UHitBoxComponent::BlockDamageCheck);
	}
	else if (HitBoxType == EHitBoxType::E_Hit)
	{
		OnComponentBeginOverlap.AddDynamic(this, &UHitBoxComponent::OverlapHitCheck);
		//OnComponentHit.AddDynamic(this, &UHitBoxComponent::BlockHitCheck);

	}
	if (HitBoxType == EHitBoxType::E_Push_Strong)
	{
		OnComponentBeginOverlap.AddDynamic(this, &UHitBoxComponent::OverlapPushCheck);
		OnComponentEndOverlap.AddDynamic(this, &UHitBoxComponent::EndPushCheck);
		OnComponentHit.AddDynamic(this, &UHitBoxComponent::BlockPushCheck);
		//OnComponentHit.AddDynamic(this, &UHitBoxComponent::BlockHitCheck);

	}
}

void UHitBoxComponent::InitByHitBoxType(EHitBoxType type)
{
	HitBoxType = type;
	switch (type)
	{
	case EHitBoxType::E_None:
		SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		break;
	case EHitBoxType::E_Hit:
		SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
		SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);
		ShapeColor = FColor::Red;
		break;
	case EHitBoxType::E_Damage:
		SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
		ShapeColor = FColor::Green;
		break;
	case EHitBoxType::E_Throw_Hit:
		ShapeColor = FColor::Magenta;
		SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
		SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel10, ECollisionResponse::ECR_Overlap);
		break;
	case EHitBoxType::E_Throw_Damage:
		ShapeColor = FColor::Cyan;
		SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel10);
		SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap);
		break;
	case EHitBoxType::E_Push_Weak:
		ShapeColor = FColor::Black;
		SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel5);
		break;
	case EHitBoxType::E_Push_Strong:
		ShapeColor = FColor::Yellow;
		SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel7);
		SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5,ECollisionResponse::ECR_Overlap);
		SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel6,ECollisionResponse::ECR_Overlap);
		SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel7,ECollisionResponse::ECR_Overlap);
		SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel8,ECollisionResponse::ECR_Ignore);
		//SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Block);
		break;
	default:
		break;
	}
}

void UHitBoxComponent::VisibleHitBoxInGame(bool bHidden)
{
	AInGameGameMode* GameMode = Cast<AInGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode))
	{
		if (GameMode->bVisbleHitBoxDebug)
		{
			SetHiddenInGame(bHidden);
		}
		else
		{
			SetHiddenInGame(true);
		}
	}
	
}


void UHitBoxComponent::OverlapHitCheck(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{


}

void UHitBoxComponent::OverlapDamageCheck(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != GetOwner())
	{
		Cast<APlayerCharacter>(GetOwner())->DamageCheck(OtherActor, OtherComp);
	}
}

void UHitBoxComponent::BlockHitCheck(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("Hit")));

}

void UHitBoxComponent::BlockDamageCheck(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("Damage")));

	if (OtherActor != GetOwner())
	{
		HitLocation = Hit.ImpactPoint;
		Cast<APlayerCharacter>(GetOwner())->DamageCheck(OtherActor, OtherComp);
	}
}

void UHitBoxComponent::OverlapPushCheck(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != GetOwner())
	{
		if (IsValid(Cast<APlayerCharacter>(GetOwner())))
		{
			if (IsValid(Cast<APlayerCharacter>(OtherActor)))
			{
				Cast<APlayerCharacter>(GetOwner())->PushCheck(OtherActor, OtherComp);
			}
			if (IsValid(Cast<AWallActor>(OtherActor)))
			{
				Cast<APlayerCharacter>(GetOwner())->bBlockWall = true;
			}
		}
	}

}

void UHitBoxComponent::BlockPushCheck(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Collision")));
	//if (OtherActor != GetOwner())
	//{
	//	if (IsValid(Cast<APlayerCharacter>(GetOwner())))
	//	{
	//		Cast<APlayerCharacter>(GetOwner())->GetPush(EGetDamagedType::E_None, Cast<APlayerCharacter>(OtherActor), Cast<UHitBoxComponent>(HitComponent), 3);
	//	}
	//}
}

void UHitBoxComponent::EndPushCheck(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(Cast<APlayerCharacter>(GetOwner())))
	{
		if (IsValid(Cast<APlayerCharacter>(OtherActor)))
		{
			Cast<APlayerCharacter>(GetOwner())->EndPush();
		}
		if (IsValid(Cast<AWallActor>(OtherActor)))
		{
			Cast<APlayerCharacter>(GetOwner())->bBlockWall = false;
		}
	}
	
}
