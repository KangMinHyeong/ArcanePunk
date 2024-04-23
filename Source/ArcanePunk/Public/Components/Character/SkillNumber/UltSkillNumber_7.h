// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "UltSkillNumber_7.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API UUltSkillNumber_7 : public USkillNumberBase
{
	GENERATED_BODY()
public:	
	UUltSkillNumber_7();
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void PlaySkill() override;
	
	virtual void OnSkill() override;

	virtual void Activate_Skill() override;

	virtual void SkillEnd() override;	

	virtual void UpdateSkillData() override;

private:
	void Spawn_UltSkill7();	
	void PlayerCollisionChange(bool NewBool);
	void CameraLagStart();
	
private:
	// TWeakObjectPtr<ABlackHole> BlackHole;

	float UltSkill7_TargetRange = 100.0f;
	// float UltSkill4_TargetRange_Origin = 120.0f;

	float UltSkill7_LimitDistance = 500.0f;	

	float TeleportDelay = 0.2f;

	float TeleportDelay_2 = 0.2f;

	FTimerHandle TeleportDelayTimerHandle;

	FVector TeleportLocation;

	float DamageCoefficient = 1.0f;
};
