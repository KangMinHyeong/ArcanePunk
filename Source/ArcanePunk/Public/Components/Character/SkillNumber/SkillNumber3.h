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
public:	
	USkillNumber3();
protected:
	virtual void BeginPlay() override;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void PlaySkill() override;
	
	virtual void Remove_Skill() override;

	virtual void OnSkill() override;

	virtual void UpdateSkillData() override;
	
private:
	virtual void Spawn_SkillRange() override;
	void SpawnAttackSphere();
	virtual void AddAbilityList() override;

private:
	FVector MaxDist = FVector(0,0,0);

	float Skill3_TargetRange = 150.0f;

	float Skill3_LimitDist = 1000.0f;
};
