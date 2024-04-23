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
	USkillNumber12();
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
};
