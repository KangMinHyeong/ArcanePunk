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
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType = ESkillTypeState::Type_None) override;	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) override;

	virtual void OnSkill() override;

	virtual void Remove_Skill() override;

	void Activate_Skill4();

private:
	void Spawn_Skill4();
	virtual void AddAbilityList() override;
	
	UPROPERTY()
	float Skill4_TargetRange = 350.0f;

	UPROPERTY()
	float Skill4_LimitDistance = 550.0f;

};
