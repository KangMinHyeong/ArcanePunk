// Fill out your copyright notice in the Description page of Project Settings.

#include "Skill/SkillDataManagers/UProjectileSkillActorDataManager.h"
#include "Engine/DataTable.h"
#include "UObject/UnrealType.h"
#include "Character/SkillRange/APSkillRange.h"
#include "Character/SkillRange/APSkillRange_TwoCircle.h"
#include "Character/SkillRange/APSkillRange_Target.h"
#include "Character/SkillRange/APSkillRange_Circle.h"
#include "Character/SkillRange/APSkillRange_Arrow.h"

void UProjectileSkillActorDataManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ReadSkillData();

	// static ConstructorHelpers::FClassFinder<AAPSkillRange> SkillRangeClass(TEXT("/Game/Blueprints/Character/SkillRange/BP_SkilRange"));
	// if(SkillRangeClass.Succeeded()) SkillRange = SkillRangeClass.Class;
	//
	// static ConstructorHelpers::FClassFinder<AAPSkillRange_Target> SkillRange_TargetClass(TEXT("/Game/Blueprints/Character/SkillRange/BP_SkillRange_Target"));
	// if(SkillRange_TargetClass.Succeeded()) SkillRange_Target = SkillRange_TargetClass.Class;
	//
	// static ConstructorHelpers::FClassFinder<AAPSkillRange_Arrow> SkillRange_ArrowClass(TEXT("/Game/Blueprints/Character/SkillRange/BP_APSkillRange_Arrow"));
	// if(SkillRange_ArrowClass.Succeeded()) SkillRange_Arrow = SkillRange_ArrowClass.Class;
	//
	// static ConstructorHelpers::FClassFinder<AAPSkillRange_Arrow> SkillRange_SquareClass(TEXT("/Game/Blueprints/Character/SkillRange/BP_APSkillRange_Square"));
	// if(SkillRange_SquareClass.Succeeded()) SkillRange_Square = SkillRange_SquareClass.Class;
	//
	// static ConstructorHelpers::FClassFinder<AAPSkillRange_Circle> SkillRange_CircleClass(TEXT("/Game/Blueprints/Character/SkillRange/BP_SkillRange_AroundCircle"));
	// if(SkillRange_CircleClass.Succeeded()) SkillRange_Circle = SkillRange_CircleClass.Class;
	//
	// static ConstructorHelpers::FClassFinder<AAPSkillRange_TwoCircle> SkillRange_TWoCircleClass(TEXT("/Game/Blueprints/Character/SkillRange/BP_APSkillRange_TwoCircle"));
	// if(SkillRange_TWoCircleClass.Succeeded()) SkillRange_TWoCircle = SkillRange_TWoCircleClass.Class;
	//
	// static ConstructorHelpers::FClassFinder<AAPSkillRange_Arrow> SkillRange_SectorCircleClass(TEXT("/Game/Blueprints/Character/SkillRange/BP_APSkillRange_SectorCircle"));
	// if(SkillRange_SectorCircleClass.Succeeded()) SkillRange_SectorCircle = SkillRange_SectorCircleClass.Class;
}

// Unreal Runtime에서 마우스 커서 변경시 변경이 되지 않거나 커서 이동을 해야 변경되는 오류를 수정해주는 코드
void UProjectileSkillActorDataManager::CursorImmediately()
{
	auto& App = FSlateApplication::Get();
	App.SetAllUserFocusToGameViewport();
	App.QueryCursor();
}

void UProjectileSkillActorDataManager::ReadSkillData()
{
	FString dataTablePath = TEXT("/Game/DataTable/Skill/ProjectileSkillActorDataTable.ProjectileSkillActorDataTable");

	UDataTable* skillDataTable = LoadObject<UDataTable>(nullptr, *dataTablePath);
	if (!skillDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("ProjectileSkillActorDataTable is not set."));
		return;
	}
	
	FString ContextString = "Validating Skill Data";

	TArray<FName> RowNames = skillDataTable->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		if (FSkillActorData* SkillRow = skillDataTable->FindRow<FSkillActorData>(RowName, ContextString))
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

void UProjectileSkillActorDataManager::CheckForInvalidData(FSkillActorData* SkillRow, const FString& Context)
{
	
}

FSkillActorData UProjectileSkillActorDataManager::GetSkillData(FName skillName)
{
	if(!SkillDataMap.Contains(skillName))
	{
		FSkillActorData emptySkillData;
		return emptySkillData;
	}
	
	return SkillDataMap[skillName];
}
