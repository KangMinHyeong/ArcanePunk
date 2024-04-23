// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "UltSkillNumber_10.generated.h"

class ATerminator;

UCLASS()
class ARCANEPUNK_API UUltSkillNumber_10 : public USkillNumberBase
{
	GENERATED_BODY()
public:	
	UUltSkillNumber_10();
protected:
	virtual void BeginPlay() override;
public:
	virtual void PlaySkill() override;

	virtual void OnSkill() override;
	
	virtual void Activate_Skill() override;

	virtual void SkillEnd() override;

	virtual void UpdateSkillData() override;
private:
	TWeakObjectPtr<ATerminator> Terminator;	
};
