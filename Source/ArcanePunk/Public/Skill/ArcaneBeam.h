// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "ArcaneBeam.generated.h"

class UNiagaraSystem;
class UBoxComponent;
class UNiagaraComponent;
class USkillNumberBase;

UCLASS()
class ARCANEPUNK_API AArcaneBeam : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	AArcaneBeam();

	virtual void DestroySKill() override;

protected:
	virtual void BeginPlay() override;	

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData) override;

	void SetBeamEffect();
	void SetBeamAttack();
	void SetDistance(float Dist) {Distance = Dist;};
	void SetWide(float Radius) {BeamRadius = Radius;};

	virtual void OnCharging() override;
	void OnChargingEnhance();
private:
	void OnChargindCheck(float DeltaTime);

protected:
	UPROPERTY(EditAnywhere)
	UBoxComponent* BeamBox;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* BeamEffect;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* ChargeEffect;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* ChargeEnhanceEffect;

	TWeakObjectPtr<UNiagaraComponent> BeamComp;
	
	UPROPERTY(EditAnywhere)
	float Distance = 1000.0f;

	UPROPERTY(EditAnywhere)
	FVector BeamScale = FVector(1,1,1);

	FVector BeamStart = FVector(0,0,0);

	FVector BeamEnd = FVector(0,0,0);

	UPROPERTY(EditAnywhere)
	float BeamRadius = 15.0f;

	FTimerHandle BeamHitLoopTimerHandle;

	UPROPERTY(EditAnywhere)
	float BeamHitLoopTime = 0.3f;
	
	TWeakObjectPtr<USkillNumberBase> Skill_5;

	UPROPERTY(EditAnywhere)
	float MaxDamageCoefficient = 3.0f;

	UPROPERTY(EditAnywhere)
	float InitIncreasingSpeed = 1.0f;

	float IncreasingSpeed = 1.0f;

	TWeakObjectPtr<UNiagaraComponent> ChargeEffectComp;
	TWeakObjectPtr<UNiagaraComponent> ChargeEnhanceEffectComp;
};
