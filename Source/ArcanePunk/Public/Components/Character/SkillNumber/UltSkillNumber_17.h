// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "UltSkillNumber_17.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API UUltSkillNumber_17 : public USkillNumberBase
{
	GENERATED_BODY()
public:	
	UUltSkillNumber_17();
protected:
	virtual void BeginPlay() override;
public:
	virtual void PlaySkill() override;

	virtual void OnSkill() override;

	virtual void Activate_Skill() override;

	virtual void SkillEnd() override;	

	virtual void UpdateSkillData() override;
	
private:

};
