// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "UltSkillNumber_11.generated.h"

class AOverload;

UCLASS()
class ARCANEPUNK_API UUltSkillNumber_11 : public USkillNumberBase
{
	GENERATED_BODY()
public:	
	UUltSkillNumber_11();
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void PlaySkill() override;
	virtual void OnSkill() override;
	virtual void SkillEnd() override;
	virtual void Enhance() override;

	virtual void Activate_Skill() override;

	virtual void UpdateSkillData() override;

private:
	void Activate_UltSkill_11();

private:
	TWeakObjectPtr<AOverload> Overload;

	float UltSkill11_LimitDistance = 500;
	
};
