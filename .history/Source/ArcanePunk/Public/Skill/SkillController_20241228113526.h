// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SkillActor.h"
#include "CoreMinimal.h"
#include "SkillController.generated.h"

/**
 * 
 */

UCLASS()
class ARCANEPUNK_API ASkillController : public APlayerController
{
	GENERATED_BODY()

public:
	ASkillController();
	
	virtual void UseSkill();

	void InitializeSkills(FName skillName, AArcanePunkCharacter* OwnerCharacter);

	void ShowSkillRange(float Range, FVector Location);

private:
	UPROPERTY()
	ASkillActor* SkillActor;
};

