// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "SkillNumber3.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API USkillNumber3 : public USkillNumberBase
{
	GENERATED_BODY()
public:
	virtual void PlaySkill() override;	

	void Cast_Skill3(FVector HitLocation);

	void SetSkill3(bool NewBool);

	bool bSkill3 = false;
};
