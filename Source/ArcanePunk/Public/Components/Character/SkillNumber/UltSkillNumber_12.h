// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "UltSkillNumber_12.generated.h"

class AArcaneReflector;

UCLASS()
class ARCANEPUNK_API UUltSkillNumber_12 : public USkillNumberBase
{
	GENERATED_BODY()
public:	
	UUltSkillNumber_12();
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void PlaySkill() override;
	virtual void OnSkill() override;
	virtual void SkillEnd() override;
	virtual void Enhance() override;

	virtual void Activate_Skill() override;

	virtual void UpdateSkillData() override;

private:
	void Activate_UltSkill_12();

private:
	TWeakObjectPtr<AArcaneReflector> ArcaneReflector;

	float UltSkill12_LimitDistance = 500;	
};
