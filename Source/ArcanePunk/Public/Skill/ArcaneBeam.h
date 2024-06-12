// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "ArcaneBeam.generated.h"

class UNiagaraSystem;
class UBoxComponent;
class UNiagaraComponent;

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
	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;

	void SetBeamEffect();
	void SetBeamAttack();
	void SetDistance(float Dist) {Distance = Dist;};
	void SetWide(float Radius) {BeamRadius = Radius;};

	virtual void OnCharging() override;
	void OnChargingEnhance();
private:
	void OnChargindCheck(float DeltaTime);
	void SetSkillComp();

	virtual void CheckSilverEnhance(uint8 AbilityNum, uint16 NestingNum) override;
	virtual void CheckGoldEnhance(uint8 AbilityNum, uint16 NestingNum) override;
	virtual void CheckPlatinumEnhance(uint8 AbilityNum, uint16 NestingNum) override;

	void FireEnd();
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
	float InitDist = 750.0f;

	UPROPERTY(EditAnywhere)
	FVector BeamScale = FVector(1,1,1);

	FVector BeamStart = FVector(0,0,0);

	FVector BeamEnd = FVector(0,0,0);

	UPROPERTY(EditAnywhere)
	float BeamRadius = 15.0f;
	float InitRadius = 50.0f;

	FTimerHandle BeamHitLoopTimerHandle;
	FTimerHandle BeamTimerHandle;

	UPROPERTY(EditAnywhere)
	float BeamHitLoopTime = 0.3f;
	
	UPROPERTY(EditAnywhere)
	float MaxDamageCoefficient = 3.0f;

	UPROPERTY(EditAnywhere)
	float InitIncreasingSpeed = 1.0f;

	float IncreasingSpeed = 1.0f;

	float OriginTime = 0.72;
	float BeamTime;

	bool AddCool = false;
	uint8 KillsNum = 0;
	float AddCoolTime = 0.0f;

	TWeakObjectPtr<UNiagaraComponent> ChargeEffectComp;
	TWeakObjectPtr<UNiagaraComponent> ChargeEnhanceEffectComp;
};
