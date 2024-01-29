// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
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

	void SetSkillAbility(TArray<ESkillAbility> SkillAbility, ESkillCategory UpdateSkillCategory);

private:
	// Gigant
	void SetOwnerScale();

	// Homing
	void SetHoming();

	void Accelrating();

	void SetProjectileMove();
	void SetTraceActor(AActor* OverlapActor = nullptr);
	void SetProjectileValue();
	UFUNCTION()
	void HomingEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	void ProjectileHoming(float DeltaTime);

	void AddImpulse(); //  Physics 활성화므로 Impulse 추가로 유도 활성화

	// Stun
	void SetStun();

public:
	UPROPERTY()
	UProjectileMovementComponent* OwnerProjectileMove;
	
	UPROPERTY()
	UProjectileMovementComponent* OriginProjectileMoveComp;

	UPROPERTY()
	float OriginProjectileSpeed = 0.0f;

	UPROPERTY()
	bool bTriggerOn = false;
	UPROPERTY()
	bool Init = true;
	
	UPROPERTY()
	TArray<AActor*> Actors;

	UPROPERTY()
	AActor* TraceActor = nullptr;

	UPROPERTY()
	FTimerHandle AccelerateTimerHandle;

	UPROPERTY()
	float TraceSpeed = 1000.0f;

	UPROPERTY()
	float HomingTime = 0.15f;

	UPROPERTY()
	ESkillCategory SkillCategory = ESkillCategory::None;

};