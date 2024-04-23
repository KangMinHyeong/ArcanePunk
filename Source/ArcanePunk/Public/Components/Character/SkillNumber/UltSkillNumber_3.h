// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "UltSkillNumber_3.generated.h"

class AArcaneExecution;

UCLASS()
class ARCANEPUNK_API UUltSkillNumber_3 : public USkillNumberBase
{
	GENERATED_BODY()
public:
	UUltSkillNumber_3();

protected:
	virtual void BeginPlay() override;

	void Spawn_UltSkill3();

public:
	virtual void PlaySkill() override;

	virtual void OnSkill() override;

	// virtual void Remove_Skill() override;

	virtual void Activate_Skill() override;
	virtual void SkillEnd() override;

	virtual void UpdateSkillData() override;

private:
	float Ult3_CircleRange = 450.0f;

	float Ult3_TargetWidth = 100.0f;
	float Ult3_TargetRange = 600.0f;

	AActor* TargetEnemy;

	TWeakObjectPtr<AArcaneExecution> ArcaneExecution;	
};
