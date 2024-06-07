// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "UltSkillNumber_18.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API UUltSkillNumber_18 : public USkillNumberBase
{
	GENERATED_BODY()
public:	
	UUltSkillNumber_18();
protected:
	virtual void BeginPlay() override;
public:
	virtual void PlaySkill() override;

	virtual void OnSkill() override;

	virtual void Activate_Skill() override;

	virtual void SkillEnd() override;	

	virtual void UpdateSkillData() override;

	virtual void OnCoolDown() override;
private:
	void ResetSkilling();

private:	
	FRotator DefaultRotationRate = FRotator(0.0f, 450.0f, 0.0f);

	float MinumumRate; // 공격속도에 반비례로 측정 : 1 / 공속;

	FTimerHandle BlastTimerHandle;

	FTimerHandle BlastEndTimerHandle;

	float BlastEndTime = 15.0f;

	float BlastMaxTime = 2.0f;
};
