// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/USkillControllerDataManager.h"

#include "Skill/SkillActorDataManager.h"
#include "Engine/DataTable.h"
#include "UObject/UnrealType.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_TwoCircle.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Character/SkillRange/APSkillRange_Circle.h"
#include "Character/SkillRange/APSkillRange_Arrow.h"

void USkillControllerDataManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ReadSkillData();
}

void USkillControllerDataManager::ReadSkillData()
{
	FString dataTablePath = TEXT("/Game/DataTable/Skill/SkillControllerDatatable.SkillControllerDatatable");

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
		if (FSkillControllerData* SkillRow = skillDataTable->FindRow<FSkillControllerData>(RowName, ContextString))
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

void USkillControllerDataManager::CheckForInvalidData(FSkillControllerData* SkillRow, const FString& Context)
{
	
}

FSkillControllerData USkillControllerDataManager::GetSkillData(FName skillName)
{
	if(!SkillDataMap.Contains(skillName))
	{
		FSkillControllerData emptySkillData;
		return emptySkillData;
	}
	
	return SkillDataMap[skillName];
}
