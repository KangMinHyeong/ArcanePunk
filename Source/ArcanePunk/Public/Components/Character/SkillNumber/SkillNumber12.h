// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "SkillNumber12.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API USkillNumber12 : public USkillNumberBase
{
	GENERATED_BODY()
public:
	virtual void PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType = ESkillTypeState::Type_None) override;	

	virtual void OnSkill() override;
	
	void Activate_Skill12();

private:
	virtual void AddAbilityList() override;	
};
