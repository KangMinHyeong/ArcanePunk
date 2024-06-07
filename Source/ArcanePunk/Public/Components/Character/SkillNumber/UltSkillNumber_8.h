// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "UltSkillNumber_8.generated.h"

class AArcaneMeteor;

UCLASS()
class ARCANEPUNK_API UUltSkillNumber_8 : public USkillNumberBase
{
	GENERATED_BODY()
public:	
	UUltSkillNumber_8();
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void PlaySkill() override;
	
	virtual void OnSkill() override;

	virtual void Activate_Skill() override;

	virtual void SkillEnd() override;	

	virtual void UpdateSkillData() override;

private:
	virtual void Spawn_SkillRange() override;
	
private:	
	float UltSkill8_TargetRange = 600.0f;
	float UltSkill8_TargetRange_2 = 400.0f;
	// float UltSkill4_TargetRange_Origin = 120.0f;

	float UltSkill8_LimitDistance = 1400.0f;	

	TWeakObjectPtr<AArcaneMeteor> ArcaneMeteor;
	
};
