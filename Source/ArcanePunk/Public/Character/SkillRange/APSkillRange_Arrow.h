// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SkillRange/APSkillRange.h"
#include "APSkillRange_Arrow.generated.h"

UCLASS()
class ARCANEPUNK_API AAPSkillRange_Arrow : public AAPSkillRange
{
	GENERATED_BODY()
public:
	AAPSkillRange_Arrow();

	virtual void Tick(float DeltaTime) override;

	virtual void SetSkill(const FSkillAbilityNestingData & SkillAbilityNestingData, USkillNumberBase* SkillComponent) override;

private:
	void ObstacleCheck();

};
