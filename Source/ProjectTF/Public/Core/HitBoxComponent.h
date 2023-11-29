// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "HitBoxComponent.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FBoxStatus
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere)
		FVector2D Center = FVector2D(60.f, 140.f);
	UPROPERTY(EditAnywhere)
		FVector2D Extent = FVector2D(32.f, 32.f);
};
/**
 * 
 */
UENUM(BlueprintType)
enum class EHitBoxType : uint8
{
	E_None = 0 UMETA(DisplayName = "None"),
	E_Hit = 1 UMETA(DisplayName = "Hit"),
	E_Damage = 2 UMETA(DisplayName = "Damage"),
	E_Throw_Hit = 3 UMETA(DisplayName = "Throw_Hit"),
	E_Throw_Damage = 4 UMETA(DisplayName = "Throw_Damage"),
	E_Push_Weak = 5 UMETA(DisplayName = "Push_Weak"),
	E_Push_Strong = 6 UMETA(DisplayName = "Push_Strong"),
};

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class PROJECTTF_API UHitBoxComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UHitBoxComponent();
protected:
	virtual void BeginPlay() override;


public:
	UFUNCTION()
	void OverlapHitCheck(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OverlapDamageCheck(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void BlockHitCheck(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void BlockDamageCheck(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void OverlapPushCheck(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void BlockPushCheck(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
		void EndPushCheck(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void InitByHitBoxType(EHitBoxType type);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ColIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EHitBoxType HitBoxType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FVector HitLocation;

	void VisibleHitBoxInGame(bool bVisible);
};