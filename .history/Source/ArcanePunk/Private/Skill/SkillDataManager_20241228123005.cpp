// Unreal Runtime에서 마우스 커서 변경시 변경이 되지 않거나 커서 이동을 해야 변경되는 오류를 수정해주는 코드
void USkillDataManager::CursorImmediately()
{
	auto& App = FSlateApplication::Get();
	App.SetAllUserFocusToGameViewport();
	App.QueryCursor();
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
