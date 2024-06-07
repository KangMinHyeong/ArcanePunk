// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "UltSkillNumber_4.generated.h"

class ABlackHole;

UCLASS()
class ARCANEPUNK_API UUltSkillNumber_4 : public USkillNumberBase
{
	GENERATED_BODY()
public:	
	UUltSkillNumber_4();
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
	TWeakObjectPtr<ABlackHole> BlackHole;

	float UltSkill4_TargetRange = 350.0f;
	// float UltSkill4_TargetRange_Origin = 120.0f;
	float UltSkill4_TargetRange_2 = 200.0f;

	float UltSkill4_LimitDistance = 600.0f;
	// float UltSkill4_LimitDistance_Origin = 500.0f;
};
