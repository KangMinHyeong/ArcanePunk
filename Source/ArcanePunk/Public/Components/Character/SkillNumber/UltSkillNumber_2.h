// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "UltSkillNumber_2.generated.h"

class ATimeRewinder;

UCLASS()
class ARCANEPUNK_API UUltSkillNumber_2 : public USkillNumberBase
{
	GENERATED_BODY()
public:
	UUltSkillNumber_2();

protected:
	virtual void BeginPlay() override;

public:
	virtual void PlaySkill() override;

	virtual void OnSkill() override;

	// virtual void Remove_Skill() override;

	virtual void Activate_Skill() override;
	virtual void SkillEnd() override;

	virtual void UpdateSkillData() override;

	virtual void OnCoolDown() override;
private:
	float Ult2_TargetRange = 320.0f;

	TWeakObjectPtr<ATimeRewinder> TimeRewinder;

};
