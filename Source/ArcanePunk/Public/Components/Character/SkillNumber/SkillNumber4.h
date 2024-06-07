// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "SkillNumber4.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API USkillNumber4 : public USkillNumberBase
{
	GENERATED_BODY()
public:	
	USkillNumber4();
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void PlaySkill() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) override;

	virtual void OnSkill() override;

	virtual void Remove_Skill() override;

	virtual void Activate_Skill() override;

	virtual void UpdateSkillData() override;

private:
	virtual void Spawn_SkillRange() override;
	virtual void AddAbilityList() override;
	
private:
	float Skill4_TargetRange = 350.0f;

	float Skill4_LimitDistance = 550.0f;

};
