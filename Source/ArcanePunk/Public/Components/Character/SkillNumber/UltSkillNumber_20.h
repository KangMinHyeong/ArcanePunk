// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/SkillNumber/SkillNumberBase.h"
#include "UltSkillNumber_20.generated.h"

class AImitator;

UCLASS()
class ARCANEPUNK_API UUltSkillNumber_20 : public USkillNumberBase
{
	GENERATED_BODY()
public:	
	UUltSkillNumber_20();
protected:
	virtual void BeginPlay() override;
public:
	virtual void PlaySkill() override;
	
	virtual void OnSkill() override;

	virtual void Activate_Skill() override;

	virtual void SkillEnd() override;	

	virtual void UpdateSkillData() override;

	void SetCopySkill(ESkillNumber CopySkillNum);


private:
	void Activate_UltSkill_20();
	void SetImitatorLocation();
	UFUNCTION()
	void CopySkillEnded(ESkillKey UpdateSkillKey, ESkillNumber CopySkillNum);

private:	
	// TWeakObjectPtr<AImitator> Imitator;
	
	TWeakObjectPtr<USkillNumberBase> CopySkill;

	uint8 SpawnNum = 6;
	uint8 RemainSpawn;

	UPROPERTY()
	TArray<AImitator*> Imitators;

	float SkillDuration = 25.0f;

	FTimerHandle UltSkill20_TimerHandle;
	FTimerHandle SpawnTimerHandle;

	float SpawnTime = 0.1f;
	float SpawnTotalTime = 1.2f;

	bool bInit = false;
};
