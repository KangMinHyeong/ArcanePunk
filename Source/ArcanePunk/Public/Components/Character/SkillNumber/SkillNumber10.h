// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "SkillNumber10.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API USkillNumber10 : public USkillNumberBase
{
	GENERATED_BODY()
public:	
	USkillNumber10();
protected:
	virtual void BeginPlay() override;
public:
	virtual void PlaySkill() override;
	
	virtual void OnSkill() override;

	virtual void Activate_Skill() override;

	virtual void UpdateSkillData() override;

	virtual bool CheckSkillCondition() override;

private:
	virtual void AddAbilityList() override;
	virtual void Spawn_SkillRange() override;

private:
	float Skill10_TargetRange = 60.0f;

	float Skill10_LimitDistance = 300.0f;

};
