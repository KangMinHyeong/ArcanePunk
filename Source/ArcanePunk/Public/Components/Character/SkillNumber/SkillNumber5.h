// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "SkillNumber5.generated.h"

class AArcaneBeam;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class ARCANEPUNK_API USkillNumber5 : public USkillNumberBase
{
	GENERATED_BODY()
public:	
	USkillNumber5();
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void PlaySkill() override;
	virtual void OnSkill() override;
	virtual void Remove_Skill() override;
	virtual void Activate_Skill() override;
	virtual void SkillEnd() override;

	virtual void UpdateSkillData() override;
	
	virtual void Enhance() override;
	virtual void DoubleEnhance() override;
private:
	virtual void Spawn_SkillRange() override;
	void Activate_Skill_5();

private:
	TWeakObjectPtr<AArcaneBeam> ArcaneBeam;

	float Skill5_LimitDistance = 500.0f;
	float Skill5_LimitDistance_Origin = 500.0f;

	float Skill5_Wide = 50.0f;
	float Skill5_Wide_Origin = 50.0f;
		

	// TWeakObjectPtr<UNiagaraComponent> ChargeEffectComp;
	// TWeakObjectPtr<UNiagaraComponent> ChargeEnhanceEffectComp;

	// float DamageCoefficient = 1.0f;

	// float MaxDamageCoefficient = 5.0f; 

	// float IncreasingSpeed = 0.25f; 

	// float InitIncreasingSpeed = 0.0f;

};
