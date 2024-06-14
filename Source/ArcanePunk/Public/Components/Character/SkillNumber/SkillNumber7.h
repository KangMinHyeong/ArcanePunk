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
public:	
	USkillNumber7();
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void PlaySkill() override;
	
	virtual void OnSkill() override;

	virtual void Remove_Skill() override;

	virtual void Activate_Skill() override;
	virtual void SkillEnd() override;

	virtual void UpdateSkillData() override;
	
private:
	virtual void AddAbilityList() override;
	virtual void Spawn_SkillRange() override;	

	void CheckDoubleCutter();

private:
	float Skill7_LimitDistance = 1350.0f / 2.0f;
	float Skill7_LimitDistance_Origin = 1350.0f / 2.0f;

	float Skill7_Wide = 100.0f / 2.0f;
	float Skill7_Wide_Origin = 100.0f / 2.0f;

	TWeakObjectPtr<AArcaneCutter> ArcaneCutter;

	bool bDoubleCutter = false;
	float DoubleCutterDamaged = 0.3f;
	float DoubleCutterDamaged_Origin = 0.3f;
	float DoubleCutDelay = 0.21f;

	FTimerHandle Skill7_TimerHandle;
};
