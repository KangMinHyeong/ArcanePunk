// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "SkillNumber2.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API USkillNumber2 : public USkillNumberBase
{
	GENERATED_BODY()
public:
	virtual void PlaySkill() override;

	void MarkingOn(FVector Location, AActor* OtherActor, float Time);

	void Activate_Skill2();
	
	void MarkErase();

private:
	FTimerHandle MarkTimerHandle;

	bool bMark = false;
	AActor* MarkedActor;
	FVector MarkingLocation;

};
