// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "SkillNumber3.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API USkillNumber3 : public USkillNumberBase
{
	GENERATED_BODY()
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void PlaySkill(uint8 SkillType = 0) override;	

	void Remove_Skill3();
	void Cast_Skill3();


private:
	void Spawn_Skill3();
	void CheckingOtherSkill();
	void SpawnAttackSphere(bool IsOutRange);

private:
	UPROPERTY()
	class AAPSpawnPointBase* Skill3_SpawnPoint;

	UPROPERTY()
	class AAPSpawnPointBase* Skill3Range_SpawnPoint;

	bool Skilling = false;

	FVector MaxDist = FVector(0,0,0);
};
