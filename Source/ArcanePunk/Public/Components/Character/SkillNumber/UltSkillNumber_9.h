// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "UltSkillNumber_9.generated.h"

class ACarpetBoom;

UCLASS()
class ARCANEPUNK_API UUltSkillNumber_9 : public USkillNumberBase
{
	GENERATED_BODY()
public:	
	UUltSkillNumber_9();
protected:
	virtual void BeginPlay() override;

public:
	virtual void PlaySkill() override;
	
	virtual void OnSkill() override;

	virtual void Activate_Skill() override;
	virtual void SkillEnd() override;

	virtual void UpdateSkillData() override;

private:
	void Spawn_UltSkill9();	
	void SetBoomLocation();
	
private:
	float UltSkill9_LimitDistance = 1000.0f;
	// float UltSkill6_LimitDistance_Origin = 800.0f;

	float UltSkill9_Width = 300.0f;	
	float UltSkill9_Length = 1500.0f;
	// float UltSkill6_Width_Origin = 150.0f;	

	TWeakObjectPtr<ACarpetBoom> CarpetBoom;
};
