// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "SkillNumber7.generated.h"

class AArcaneCutter;

UCLASS()
class ARCANEPUNK_API USkillNumber7 : public USkillNumberBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType = ESkillTypeState::Type_None) override;	

	virtual void OnSkill() override;

	virtual void Remove_Skill() override;

	void Activate_Skill7();
	void Skill7_End();
	
private:
	virtual void AddAbilityList() override;
	void Spawn_Skill7();	

	UPROPERTY()
	float Skill7_LimitDistance = 1350.0f / 2.0f;

	UPROPERTY()
	float Skill7_Wide = 100.0f / 2.0f;

	UPROPERTY()
	AArcaneCutter* ArcaneCutter;
};
