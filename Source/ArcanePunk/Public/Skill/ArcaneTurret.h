// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "ArcaneTurret.generated.h"

class USkeletalMeshComponent;
class UCapsuleComponent;
class UParticleSystem;
class ATurretAmmo;
class ATurretBeam;

UCLASS()
class ARCANEPUNK_API AArcaneTurret : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	AArcaneTurret();

protected:
	virtual void BeginPlay() override;	
	virtual void DestroySKill() override;
public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;
	virtual void CheckSilverEnhance(uint8 AbilityNum, uint16 NestingNum) override;
	virtual void CheckGoldEnhance(uint8 AbilityNum, uint16 NestingNum) override;
	virtual void CheckPlatinumEnhance(uint8 AbilityNum, uint16 NestingNum) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	
private:
	void AutoAttack();
	AActor* FindEnemy();
	void Fire();
	void EnhanceFire();

	void SpawnDamageText(AController* EventInstigator, float Damage);

protected:
	UPROPERTY(EditAnywhere)
	UCapsuleComponent* TurretCapsule;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TurretRoot;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* RotateMesh;

	UPROPERTY(EditAnywhere)
	float HP = 100.0f;

	UPROPERTY(EditAnywhere)
	UParticleSystem* DeadEffect;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> DamageTextClass;

	UPROPERTY(EditAnywhere)
	FVector DeadEffectSize = FVector(0.5f, 0.5f, 0.5f);

	UPROPERTY(EditAnywhere)
	float AttackDistance = 1000.0f;

	UPROPERTY(EditAnywhere)
	TWeakObjectPtr<AActor> Enemy;

	UPROPERTY(EditAnywhere)
	float RotateSpeed = 20.0f;

	FTimerHandle FireTimerHandle;
	FTimerHandle SpawnLoopTimerHandle;

	UPROPERTY(EditAnywhere)
	float FireRate = 3.0f;

	UPROPERTY(EditAnywhere)
	float SpawnLoopRate = 0.35f;

	int32 SpawnAmmoNum = 1;
	int32 OriginAmmoNum = 0;

	UPROPERTY(EditAnywhere)
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ATurretAmmo> ProjectileClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ATurretBeam> BeamClass;

	bool bDeadStun = false;

	UPROPERTY(EditAnywhere)
	float DeadStunRadius = 100.0f;

	bool bGodMode = false;

	bool bEnhance = false;
	uint8 EnhanceCount = 0;
	UPROPERTY(EditAnywhere)
	float EnhanceTerm = 0.15f;
	FTimerHandle EnhanceTimerHandle;

	// 소환 이펙트 후에 추가	
};
