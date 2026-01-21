// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillDataStructs/FSkillControllerData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "USkillControllerDataManager.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API USkillControllerDataManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
private:
	// 스킬 데이터 맵
	TMap<FName, FSkillControllerData> SkillDataMap;
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void ReadSkillData();
	FSkillControllerData GetSkillData(FName skillName);
	
private:
	void CheckForInvalidData(FSkillControllerData* SkillRow, const FString& Context);
};
