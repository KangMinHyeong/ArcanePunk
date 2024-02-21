// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "SkillNumber11.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API USkillNumber11 : public USkillNumberBase
{
	GENERATED_BODY()
public:
	virtual void PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType = ESkillTypeState::Type_None) override;	

	virtual void OnSkill() override;

	void Activate_Skill11();

private:
	virtual void AddAbilityList() override;
};
