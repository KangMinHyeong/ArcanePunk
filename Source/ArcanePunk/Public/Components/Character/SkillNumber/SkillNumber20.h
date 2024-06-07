// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "SkillNumber20.generated.h"


UCLASS()
class ARCANEPUNK_API USkillNumber20 : public USkillNumberBase
{
	GENERATED_BODY()
public:	
	USkillNumber20();
protected:
	virtual void BeginPlay() override;
public:
	virtual void PlaySkill() override;
	
	virtual void OnSkill() override;

	virtual void Activate_Skill() override;

	virtual void UpdateSkillData() override;

	virtual bool CheckSkillCondition() override;

private:
	virtual void Spawn_SkillRange() override;

private:
	float Skill20_Wide = 30.0f;

	float Skill20_LimitDistance = 700.0f;	
};
