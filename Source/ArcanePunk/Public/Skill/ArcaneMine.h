// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "ArcaneMine.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class ARCANEPUNK_API AArcaneMine : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	AArcaneMine();

protected:
	virtual void BeginPlay() override;
	virtual void DestroySKill() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;
	virtual void CheckSilverEnhance(uint8 AbilityNum, uint16 NestingNum) override;
	virtual void CheckGoldEnhance(uint8 AbilityNum, uint16 NestingNum) override;
	virtual void CheckPlatinumEnhance(uint8 AbilityNum, uint16 NestingNum) override;
	
	void SetTarget(FVector TargetComponent);
	FORCEINLINE void SetExplosionRadius(float NewRadius) {ExplosionRadius = NewRadius;};
	FORCEINLINE bool IsExploaionEnable() const {return bExploaionEnable;};
	void Explosion();

private:
	UFUNCTION()
	void OnHitting(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void BintHit();

private:
	UPROPERTY(EditAnywhere)
	USphereComponent* MineTrigger;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MineMesh;	

	UPROPERTY(EditAnywhere)
	UParticleSystem* ExplosionEffect;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* MineMoveComp;

	UPROPERTY(EditAnywhere)
	float MineSpeed = 750.0f;

	float ExplosionRadius = 250.0f;

	UPROPERTY(EditAnywhere)
	FVector ExplosionScale = FVector(1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere)
	float DOT = 2.0f;
	UPROPERTY(EditAnywhere)
	float DOT_Time = 10.1f;
	UPROPERTY(EditAnywhere)
	float DOT_InRate = 2.0f;

	bool bExploaionEnable = false;

	FVector CurrentLocation;
	FVector TargetLocation;

};
