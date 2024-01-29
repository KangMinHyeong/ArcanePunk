// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "SkillNumber5.generated.h"

class AArcaneBeam;

UCLASS()
class ARCANEPUNK_API USkillNumber5 : public USkillNumberBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType = ESkillTypeState::Type_None) override;	

	virtual void OnSkill() override;

	virtual void Remove_Skill() override;

	void Activate_Skill5();
	void Skill5_End();
	
private:
	virtual void AddAbilityList() override;
	void Spawn_Skill5();
	void OnSkill5();

	UPROPERTY()
	AArcaneBeam* ArcaneBeam;

	UPROPERTY()
	float Skill5_LimitDistance = 1350.0f / 2.0f;

	UPROPERTY()
	float Skill5_Wide = 100.0f / 2.0f;
};
