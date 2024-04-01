// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillDataTable/SkillDataTable.h"
#include "UObject/Interface.h"
#include "SkillInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USkillInterface : public UInterface
{
	GENERATED_BODY()
};


class ARCANEPUNK_API ISkillInterface
{
	GENERATED_BODY()

public:
	virtual void SetSkill(FSkillAbilityNestingData SkillAbilityNestingData);
};
