// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "BlackHole.generated.h"

class USphereComponent;
class UNiagaraComponent;

UCLASS()
class ARCANEPUNK_API ABlackHole : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	ABlackHole();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;

	void SetHoleRadius(float Radius, float Radius_2);

	virtual void DestroySKill() override;

private:
    void OnGravity();
	void OnDamage();
	void OnSlow();
	void UpdateSlowPercent(FVector EnemyLocation);
	void SetBlackHoleSize();

private:
	UPROPERTY(EditAnywhere)
	USphereComponent* GravityTrigger;

	UPROPERTY(EditAnywhere)
	USphereComponent* DamageTrigger;	

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* BlackHoleEffect;

	UPROPERTY(EditAnywhere)
	float LifeTime = 6.0f;
	UPROPERTY(EditAnywhere)
	float CastingTime = 1.0f;
	UPROPERTY(EditAnywhere)
	float GravitySpeed = 500.0f;

	UPROPERTY(EditAnywhere)
	float DamageRate = 0.5f;
	UPROPERTY(EditAnywhere)
	float SlowRate = 0.1f;

	float OutSize = 0.0f;
    float InSize = 0.0f;

	FTimerHandle TriggerTimerHandle;
	FTimerHandle DamageTimerHandle;
	FTimerHandle SlowTimerHandle;
};
