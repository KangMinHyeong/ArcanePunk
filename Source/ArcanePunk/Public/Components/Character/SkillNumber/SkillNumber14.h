// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "SkillNumber14.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API USkillNumber14 : public USkillNumberBase
{
	GENERATED_BODY()
public:
	virtual void PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType = ESkillTypeState::Type_None) override;	

	virtual void OnSkill() override;

	virtual void Remove_Skill() override;	

	void Activate_Skill14();

private:
	virtual void AddAbilityList() override;
	void Spawn_Skill14();

	UPROPERTY()
	float Skill14_Wide = 150.0f;

	UPROPERTY()
	float Skill14_LimitDistance = 600.0f;



};
