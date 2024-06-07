// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "SkillNumber19.generated.h"

class AMoonSlash;

UCLASS()
class ARCANEPUNK_API USkillNumber19 : public USkillNumberBase
{
	GENERATED_BODY()
public:	
	USkillNumber19();
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
	void SetSlashInform();	
	
private:	
	TWeakObjectPtr<AMoonSlash> MoonSlash;	

	float Skill19_LimitDistance = 500.0f;
	// float Skill19_Width;
	float Skill19_Angle = 0.4f;
};
