// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "SkillNumber6.generated.h"

class AShouting;

UCLASS()
class ARCANEPUNK_API USkillNumber6 : public USkillNumberBase
{
	GENERATED_BODY()
public:	
	USkillNumber6();
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
	void AddAbilityList();	
	void CheckLionHowling();

private:
	TWeakObjectPtr<AShouting> Shouting;	

	bool LionHowling = false;

	uint8 LionHowlingCount = 0;
};
