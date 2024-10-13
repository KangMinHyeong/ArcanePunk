#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Engine/DataTable.h"
#include "Widgets/Input/SComboBox.h"

enum class ESkillKey : uint8;

class ARCANEPUNKEDITORTOOLS_API FSkillEditorTool : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(FSkillEditorTool) {}
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);
	void OnPostPIEStarted(bool bIsSimulating);
	void OnPIEEnd(bool bIsSimulating);
	void OnToolEnabledToggled(ECheckBoxState NewState);
	ECheckBoxState IsToolEnabledChecked() const;
	bool IsToolEnabled() const;

private:
	void PopulateSkillDropdown();
	FText GetSelectedSkillText(ESkillKey SkillKey) const;
	void OnSkillSelected(TSharedPtr<FName> NewSelection, ESelectInfo::Type SelectInfo, ESkillKey SkillKey);
	TSharedRef<SComboBox<TSharedPtr<FName>>> CreateSkillComboBox(ESkillKey SkillKey);
	TSharedRef<SWidget> GenerateSkillComboBoxItem(TSharedPtr<FName> InItem);
	FReply EquipSelectedSkill();
	UWorld* GetEditorWorld();

private:
	UDataTable* SkillDataTable = nullptr;
	bool bIsToolEnabled = false;
	
	TArray<TSharedPtr<FName>> SkillList;
	TMap<ESkillKey, TSharedPtr<SComboBox<TSharedPtr<FName>>>> SkillDropdown;
	TMap<ESkillKey, TSharedPtr<FName>> SelectedSkills;
	TSharedPtr<SCheckBox> EnableToolCheckBox;
	TSharedPtr<SButton> EquipButton;
};

