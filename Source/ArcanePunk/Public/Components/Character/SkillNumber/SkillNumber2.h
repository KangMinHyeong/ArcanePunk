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
protected:
	virtual void BeginPlay() override;
public:
	virtual void PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType = ESkillTypeState::Type_None) override;	

	virtual void MarkingOn(AActor* OtherActor, float Time) override;

	virtual void Activate_Skill() override;
	
	virtual void MarkErase() override;

	virtual void OnSkill() override;

	virtual void Remove_Skill() override;

private:
	virtual void AddAbilityList() override;

private:
	FTimerHandle MarkTimerHandle;

	TWeakObjectPtr<AActor> MarkedActor;
	FVector MarkingLocation;

};
