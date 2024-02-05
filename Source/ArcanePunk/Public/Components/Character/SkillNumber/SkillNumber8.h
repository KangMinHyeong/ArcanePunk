// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "SkillNumber8.generated.h"

class AArcaneBall;

UCLASS()
class ARCANEPUNK_API USkillNumber8 : public USkillNumberBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType = ESkillTypeState::Type_None) override;	

	virtual void OnSkill() override;

	virtual void Remove_Skill() override;

	void Activate_Skill8();
	void Skill8_End();

private:
	virtual void AddAbilityList() override;
	void Spawn_Skill8();	

	UPROPERTY()
	float Skill8_LimitDistance = 1350.0f / 2.0f;

	UPROPERTY()
	float Skill8_Wide = 200.0f / 2.0f;	

	UPROPERTY()
	AArcaneBall* ArcaneBall;

};
