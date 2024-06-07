// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "UltSkillNumber_19.generated.h"


UCLASS()
class ARCANEPUNK_API UUltSkillNumber_19 : public USkillNumberBase
{
	GENERATED_BODY()
public:	
	UUltSkillNumber_19();
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
	void SkillSetting(bool Start);

private:
	float UltSkill19_TargetRange = 60.0f;
	float UltSkill19_LimitDistance = 300.0f;	

	FTimerHandle AlterEgoTimerHandle;
	float AlterEgoDelay = 0.5f;

	FVector AlterEgoEffectScale = FVector(1.35f, 1.35f, 1.35f);
};
