// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "ArcaneMeteor.generated.h"

class USphereComponent;
class UNiagaraComponent;

UCLASS()
class ARCANEPUNK_API AArcaneMeteor : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	AArcaneMeteor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData) override;

	void SetRadius(float Radius_1, float Radius_2);

	virtual void DestroySKill() override;

private:
	void OnMeteorDrop();
	void BurnDamage();

private:
	UPROPERTY(EditAnywhere)
	USphereComponent* DamageTrigger;

	UPROPERTY(EditAnywhere)
	USphereComponent* DamageEnhanceTrigger;	

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* MeteorEffect;

	UPROPERTY(EditAnywhere)
	float EnhanceDamageCoefficient = 2.0f;

	UPROPERTY(EditAnywhere)
	float CastingDelay = 1.0f;

	UPROPERTY(EditAnywhere)
	float BurnRate = 1.0f;

	UPROPERTY(EditAnywhere)
	uint8 BurnNumber = 4;

	UPROPERTY(EditAnywhere)
	float DotDamage = 5.0f;

	FTimerHandle MeteorTimerHandle;

	float TriggerRadius_1 = 0.0f;
	float TriggerRadius_2 = 0.0f;
	float MeteorSize = 0.1f;
};