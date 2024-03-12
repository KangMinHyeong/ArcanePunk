// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "SkillNumber3.generated.h"

class AAPSpawnPointBase;

UCLASS()
class ARCANEPUNK_API USkillNumber3 : public USkillNumberBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void PlaySkill(ESkillKey WhichKey, ESkillTypeState SkillType = ESkillTypeState::Type_None) override;	

	virtual void Remove_Skill() override;

	virtual void OnSkill() override;
	
private:
	void Spawn_Skill3();
	void SpawnAttackSphere();
	virtual void AddAbilityList() override;

private:
	FVector MaxDist = FVector(0,0,0);

	float Skill3_TargetRange = 150.0f;
};
