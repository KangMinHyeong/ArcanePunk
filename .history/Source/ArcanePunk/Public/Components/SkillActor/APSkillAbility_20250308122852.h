// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SkillDataTable/SkillDataTable.h"
#include "Components/ActorComponent.h"
#include "APSkillAbility.generated.h"

class UProjectileMovementComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPSkillAbility : public UActorComponent
{
	GENERATED_BODY()
public:	
	UAPSkillAbility();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Coefficient Calculate
	void Coefficient_Add(float &Current, float Add, uint16 NestingNum);
	void Coefficient_Add(int32 & Current, float Add, uint16 NestingNum);
	float Coefficient_Add_Return(float Current, float Add, uint16 NestingNum);
	int32 Coefficient_Add_Return(int32 Current, float Add, uint16 NestingNum);

	void Coefficient_Multiple(float &Current, float Coefficient, uint16 NestingNum);
	float Coefficient_Multiple_Return(float Current, float Coefficient, uint16 NestingNum);
	
	void Coefficient_AddMultiple(float &Current, float Coefficient, uint16 NestingNum);
	void Coefficient_AddMultiple(int32 &Current, float Coefficient, uint16 NestingNum);
	void Coefficient_SubtractMultiple(float &Current, float Coefficient, uint16 NestingNum);

	// Gigant
	void GigantGradually(float Coefficient, uint16 NestingNum);
	float SizeUp_Multiple(float Coefficient, uint16 NestingNum);
	// void SetOwnerScale();

	// // Homing
	// void SetHoming();

	// void Accelrating();

	// void SetProjectileMove();
	// void SetTraceActor(AActor* OverlapActor = nullptr);
	// void SetProjectileValue();
	// UFUNCTION()
	// void HomingEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	// void ProjectileHoming(float DeltaTime);

	// void AddImpulse(); //  Physics 활성화므로 Impulse 추가로 유도 활성화

	// // Stun
	// void SetStun();

public:
	// UPROPERTY(EditAnywhere)
	// UProjectileMovementComponent* OwnerProjectileMove;
	
	// UPROPERTY(EditAnywhere)
	// UProjectileMovementComponent* OriginProjectileMoveComp;

	// float OriginProjectileSpeed = 0.0f;

	// bool bTriggerOn = false;

	// bool Init = true;

	// UPROPERTY()
	// TArray<AActor*> Actors;

	// TWeakObjectPtr<AActor> TraceActor;

	// FTimerHandle AccelerateTimerHandle;

	// float TraceSpeed = 1000.0f;

	// float HomingTime = 0.15f;

	// ESkillCategory SkillCategory = ESkillCategory::None;

	/* version 04.03 */
	
	// Gigant
	float GigantCoefficient = 1.0f;
	FVector CurrentScale;
	FVector TargetScale;

	// 
};