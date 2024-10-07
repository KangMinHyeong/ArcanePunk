// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/APSkillActorBase.h"
#include "Overload.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class ARCANEPUNK_API AOverload : public AAPSkillActorBase
{
	GENERATED_BODY()
public:	
	AOverload();

protected:
	virtual void BeginPlay() override;	

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetSkill(const FSkillAbilityNestingData & SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;
	void StartFireCheck();
	void StartFire();
	virtual void DestroySKill() override;	

	void SetRange(float Radius);
private:
	void FireChecking(float DeltaTime);
	void ChargeEffectEnd();

private:
	UPROPERTY(EditAnywhere)
	USphereComponent* OverloadTrigger;	

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* OverloadRangeEffect;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* OverloadFireEffect;

	float OverloadRange = 0.0f;
	float RangeSize = 0.0f;

	UPROPERTY(EditAnywhere)
	float MaxDamageCoefficient = 3.0f;

	UPROPERTY(EditAnywhere)
	float MaxTime = 3.0f;

	float ChargeSpeed = 0.0f;
};
