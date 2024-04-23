// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "UltSkillNumber_13.generated.h"

class AArcaneWave;

UCLASS()
class ARCANEPUNK_API UUltSkillNumber_13 : public USkillNumberBase
{
	GENERATED_BODY()
public:	
	UUltSkillNumber_13();
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void PlaySkill() override;
	
	virtual void OnSkill() override;

	virtual void Activate_Skill() override;

	virtual void SkillEnd() override;	

	virtual void UpdateSkillData() override;

private:
	void Spawn_UltSkill13();	

private:	
	TWeakObjectPtr<AArcaneWave> ArcaneWave;

	float UltSkill13_LimitDistance = 700.0f;

};
