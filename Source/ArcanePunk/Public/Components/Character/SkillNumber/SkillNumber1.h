// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "SkillNumber1.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API USkillNumber1 : public USkillNumberBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:
	virtual void PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType = ESkillTypeState::Type_None) override;	

	void Activate_Skill1();

	virtual void OnSkill() override;

	virtual void Remove_Skill() override;

private:
	virtual void AddAbilityList() override;
};
