// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "ArcaneTurret.generated.h"

class USkeletalMeshComponent;
class UCapsuleComponent;
class UParticleSystem;
class ATurretAmmo;

UCLASS()
class ARCANEPUNK_API AArcaneTurret : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	AArcaneTurret();

protected:
	virtual void BeginPlay() override;	

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetSkill(ESkillTypeState SkillType, TArray<ESkillAbility> SkillAbility) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	
private:
	void AutoAttack();
	AActor* FindEnemy();
	void Fire();

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
	AActor* Enemy;

	UPROPERTY(EditAnywhere)
	float RotateSpeed = 20.0f;

	FTimerHandle FireTimerHandle;

	UPROPERTY(EditAnywhere)
	float FireRate = 3.0f;

	UPROPERTY(EditAnywhere)
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ATurretAmmo> ProjectileClass;

	// 소환 이펙트 후에 추가	
};
