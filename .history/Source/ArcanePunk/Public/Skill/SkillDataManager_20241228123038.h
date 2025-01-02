// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FSkillData.h"
#include "SkillDataManager.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API USkillDataManager : public UObject
{
	GENERATED_BODY()
	
public:
	static USkillDataManager* GetInstance();

private:
	// 스킬 데이터 맵
	TMap<FName, FSkillData> SkillDataMap;

public:
	void ReadSkillData();
	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public:
	FSkillData GetSkillData(FName skillName);
	
private:
	void CheckForInvalidData(FSkillData* SkillRow, const FString& Context);
};
