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
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType = ESkillTypeState::Type_None) override;	

	virtual void OnSkill() override;

	virtual void Remove_Skill() override;

	virtual void Activate_Skill() override;
	virtual void SkillEnd() override;
	
	virtual void Enhance() override;

	virtual void RemoveEffect() override;

private:
	virtual void AddAbilityList() override;
	void Spawn_Skill5();
	void OnSkill5();
	void SpawnChargeEffect();

private:
	TWeakObjectPtr<AArcaneBeam> ArcaneBeam;

	float Skill5_LimitDistance = 1350.0f / 2.0f;

	float Skill5_Wide = 100.0f / 2.0f;

	TWeakObjectPtr<UNiagaraComponent> ChargeEffectComp;
	TWeakObjectPtr<UNiagaraComponent> ChargeEnhanceEffectComp;

	float DamageCoefficient = 1.0f;

	float MaxDamageCoefficient = 5.0f; 

	float IncreasingSpeed = 0.25f; 

	float InitIncreasingSpeed = 0.0f;

};
