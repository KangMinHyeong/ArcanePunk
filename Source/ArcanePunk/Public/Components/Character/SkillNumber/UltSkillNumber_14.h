// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "UltSkillNumber_14.generated.h"

class AWindCutter;

UCLASS()
class ARCANEPUNK_API UUltSkillNumber_14 : public USkillNumberBase
{
	GENERATED_BODY()
public:	
	UUltSkillNumber_14();
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
	void SetWindInform();
	
private:	
	TWeakObjectPtr<AWindCutter> WindCutter;

	float UltSkill14_LimitDistance = 700.0f;
	float UltSkill14_Width;
	float UltSkill14_Angle = 0.25f;
};
