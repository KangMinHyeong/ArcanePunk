// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "UltSkillNumber_6.generated.h"

class AWindRush;

UCLASS()
class ARCANEPUNK_API UUltSkillNumber_6 : public USkillNumberBase
{
	GENERATED_BODY()
public:	
	UUltSkillNumber_6();
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
	float UltSkill6_LimitDistance = 800.0f;
	// float UltSkill6_LimitDistance_Origin = 800.0f;

	float UltSkill6_Width = 150.0f;	
	// float UltSkill6_Width_Origin = 150.0f;	

	TWeakObjectPtr<AWindRush> WindRush;
};
