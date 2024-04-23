// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "SkillNumber18.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API USkillNumber18 : public USkillNumberBase
{
	GENERATED_BODY()
public:	
	USkillNumber18();
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void PlaySkill() override;
	virtual void OnSkill() override;
	virtual void SkillEnd() override;

	virtual void Activate_Skill() override;

	virtual void UpdateSkillData() override;

private:
	void Spawn_Skill18();

private:
	float Skill18_LimitDistance = 0.0f;		
};
