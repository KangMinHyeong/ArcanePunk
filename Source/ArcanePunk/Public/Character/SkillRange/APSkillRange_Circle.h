// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SkillRange/APSkillRange.h"
#include "APSkillRange_Circle.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API AAPSkillRange_Circle : public AAPSkillRange
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;


};
