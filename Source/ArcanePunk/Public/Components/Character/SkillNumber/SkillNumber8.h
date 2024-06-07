// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "SkillNumber8.generated.h"

class AArcaneBall;

UCLASS()
class ARCANEPUNK_API USkillNumber8 : public USkillNumberBase
{
	GENERATED_BODY()
public:	
	USkillNumber8();
protected:
	virtual void BeginPlay() override;

public:
	virtual void PlaySkill() override;
	
	virtual void OnSkill() override;

	virtual void Activate_Skill() override;
	virtual void SkillEnd() override;

	virtual void UpdateSkillData() override;

private:
	virtual void AddAbilityList() override;
	virtual void Spawn_SkillRange() override;

private:
	float Skill8_LimitDistance = 700.0f;
	float Skill8_LimitDistance_Origin = 700.0f;

	float Skill8_Wide = 100.0f;	
	float Skill8_Wide_Origin = 100.0f;	

	TWeakObjectPtr<AArcaneBall> ArcaneBall;

};
