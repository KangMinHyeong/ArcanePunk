// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "ArcaneBall.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UProjectileMovementComponent;


UCLASS()
class ARCANEPUNK_API AArcaneBall : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	AArcaneBall();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
	void OnHitting(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit);
	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;

	virtual void CheckSilverEnhance(uint8 AbilityNum, uint16 NestingNum) override;
	virtual void CheckGoldEnhance(uint8 AbilityNum, uint16 NestingNum) override;
	virtual void CheckPlatinumEnhance(uint8 AbilityNum, uint16 NestingNum) override;

	void UpdateBallSpeed();
	
	FORCEINLINE float GetBallSpeed() const;

	virtual void SetDeadTime(float DeadTime) override;

	void SetBallRadius(float Radius);
	void Explosion();

private:
	UFUNCTION()
	void BintOverlap();

	void SpawnGravityBall(float Radius);

	virtual void HomingOrderSet() override;
	
private:
	UPROPERTY(EditAnywhere)
	USphereComponent* BallTrigger;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BallCenter;
	
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* BallEffect;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* BallHitEffect;

	UPROPERTY(EditAnywhere)
	USphereComponent* GravityBallTrigger;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* BallMoveComp;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* HomingMoveComp;

	UPROPERTY(EditAnywhere)
	float BallSpeed = 750.0f;
	float CurrentBallSpeed;

	UPROPERTY(EditAnywhere)
	float ExplosionRadius = 250.0f;
	
	bool bDrag = false;

	float GravitySpeed = 0.0f;

	bool bGravityMode = false;
};
