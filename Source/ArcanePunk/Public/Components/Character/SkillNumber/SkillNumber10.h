// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "SkillNumber10.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API USkillNumber10 : public USkillNumberBase
{
	GENERATED_BODY()
public:
	virtual void PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType = ESkillTypeState::Type_None) override;	

	virtual void OnSkill() override;

	virtual void Remove_Skill() override;	

	void Activate_Skill10();

private:
	virtual void AddAbilityList() override;
	void Spawn_Skill10();

	UPROPERTY()
	float Skill10_TargetRange = 60.0f;

	UPROPERTY()
	float Skill10_LimitDistance = 300.0f;

	UPROPERTY()
	FVector SkillRange_TargetLocation = FVector(0,0,0);

};
