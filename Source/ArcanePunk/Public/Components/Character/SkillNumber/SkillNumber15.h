// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "SkillNumber15.generated.h"


UCLASS()
class ARCANEPUNK_API USkillNumber15 : public USkillNumberBase
{
	GENERATED_BODY()
public:	
	USkillNumber15();
protected:
	virtual void BeginPlay() override;
public:
	virtual void PlaySkill() override;

	virtual void OnSkill() override;
	
	virtual void Activate_Skill() override;

	virtual void SkillEnd() override;

	virtual void UpdateSkillData() override;	

private:
	float HPRecoveryPercent = 0.25f;
};
