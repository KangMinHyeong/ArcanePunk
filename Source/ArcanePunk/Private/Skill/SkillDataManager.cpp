// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SkillDataManager.h"
#include "Engine/DataTable.h"
#include "UObject/UnrealType.h"

USkillDataManager* USkillDataManager::GetInstance()
{
	static USkillDataManager* Instance = nullptr;
	if (!Instance)
	{
		Instance = NewObject<USkillDataManager>();
		Instance->AddToRoot();
	}

	return Instance;
}

void USkillDataManager::ReadSkillData()
{
	FString dataTablePath = TEXT("/Game/DataTable/Skill/SkillDataTable.SkillDataTable");

	UDataTable* skillDataTable = LoadObject<UDataTable>(nullptr, *dataTablePath);
	if (!skillDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("SkillDataTable is not set."));
		return;
	}
	
	FString ContextString = "Validating Skill Data";

	TArray<FName> RowNames = skillDataTable->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		FSkillData* SkillRow = skillDataTable->FindRow<FSkillData>(RowName, ContextString);
		if (SkillRow)
		{
			CheckForInvalidData(SkillRow, ContextString);
			SkillDataMap.Add(RowName, *SkillRow);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Could not find row for Skill: %s"), *RowName.ToString());
		}
	}
}

void USkillDataManager::CheckForInvalidData(FSkillData* SkillRow, const FString& Context)
{

}

void USkillDataManager::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// 데이터 테이블 변경 시 유효성 검사 수행
	ReadSkillData();
}

FSkillData USkillDataManager::GetSkillData(FName skillName)
{
	if(!SkillDataMap.Contains(skillName))
	{
		FSkillData emptySkillData;
		return emptySkillData;
	}
	
	return SkillDataMap[skillName];
}
