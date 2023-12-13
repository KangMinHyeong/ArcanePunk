// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "SkillNumber1.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API USkillNumber1 : public USkillNumberBase
{
	GENERATED_BODY()
public:
	virtual void PlaySkill() override;

	void Activate_Skill1();
};
