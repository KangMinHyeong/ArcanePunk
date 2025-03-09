// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SkillDataTable/SkillDataTable.h"
#include "UObject/Interface.h"
#include "SkillInterface.generated.h"

class USkillNumberBase;

UINTERFACE(MinimalAPI)
class USkillInterface : public UInterface
{
	GENERATED_BODY()
};


class ARCANEPUNK_API ISkillInterface
{
	GENERATED_BODY()

public:
	virtual void SetSkill(const FSkillAbilityNestingData & SkillAbilityNestingData, USkillNumberBase* SkillComponent);
};
