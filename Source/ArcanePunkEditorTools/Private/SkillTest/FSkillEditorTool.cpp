#include "SkillTest/FSkillEditorTool.h"
#include "Editor.h"

#include "ArcanePunk/Public/Character/ArcanePunkCharacter.h"

void FSkillEditorTool::Construct(const FArguments& InArgs)
{
    SkillDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/DataTable/Skill/SkillDataTable.SkillDataTable"));

    UWorld* World = GetEditorWorld();

    FEditorDelegates::PostPIEStarted.AddRaw(this, &FSkillEditorTool::OnPostPIEStarted);
    FEditorDelegates::EndPIE.AddRaw(this, &FSkillEditorTool::OnPIEEnd);
    
    ChildSlot
    [
        SNew(SVerticalBox)
        + SVerticalBox::Slot()
        .AutoHeight()
        [
            SAssignNew(EnableToolCheckBox, SCheckBox)
            .IsEnabled(World != nullptr) 
            .OnCheckStateChanged(this, &FSkillEditorTool::OnToolEnabledToggled)
            .IsChecked(this, &FSkillEditorTool::IsToolEnabledChecked)
            .Content()
            [
                SNew(STextBlock)
                .Text(FText::FromString("Enable Skill Test"))
            ]
        ]
        + SVerticalBox::Slot()
        .AutoHeight()
        [
            SNew(STextBlock).Text(FText::FromString("Select Skill Q:"))
        ]
        + SVerticalBox::Slot()
        .AutoHeight()
        [
            CreateSkillComboBox(ESkillKey::Q)
        ]
        + SVerticalBox::Slot()
        .AutoHeight()
        [
            SNew(STextBlock).Text(FText::FromString("Select Skill E:"))
        ]
        + SVerticalBox::Slot()
        .AutoHeight()
        [
            CreateSkillComboBox(ESkillKey::E)
        ]
        + SVerticalBox::Slot()
        .AutoHeight()
        [
            SNew(STextBlock).Text(FText::FromString("Select Skill R:"))
        ]
        + SVerticalBox::Slot()
        .AutoHeight()
        [
            CreateSkillComboBox(ESkillKey::R)
        ]
        + SVerticalBox::Slot()
        .AutoHeight()
        [
            SAssignNew(EquipButton, SButton)
            .Text(FText::FromString("Equip Skill"))
            .IsEnabled(this, &FSkillEditorTool::IsToolEnabled) 
            .OnClicked(this, &FSkillEditorTool::EquipSelectedSkill)
        ]
    ];
}

void FSkillEditorTool::OnPostPIEStarted(bool bIsSimulating)
{
    EnableToolCheckBox->SetEnabled(true);

    PopulateSkillDropdown();
    for(auto DropDownTuple : SkillDropdown)
    {
        DropDownTuple.Value->RefreshOptions();
    }
}

void FSkillEditorTool::OnPIEEnd(bool bIsSimulating)
{
    EnableToolCheckBox->SetIsChecked(false);
    OnToolEnabledToggled(ECheckBoxState::Unchecked);
    EnableToolCheckBox->SetEnabled(false);
}


void FSkillEditorTool::OnToolEnabledToggled(ECheckBoxState NewState)
{
    bIsToolEnabled = (NewState == ECheckBoxState::Checked);

    UWorld* World = GetEditorWorld();

    if (!World)
    {
        return;
    }
    
    APlayerController* PlayerController = World->GetFirstPlayerController();
    if (!PlayerController)
    {
        return;
    }

    AArcanePunkCharacter* PlayerCharacter = Cast<AArcanePunkCharacter>(PlayerController->GetCharacter());
    if (!PlayerCharacter)
    {
        return;
    }

    for(auto DropDownTuple : SkillDropdown)
    {
        DropDownTuple.Value->SetEnabled(bIsToolEnabled);
    }

    EquipButton->SetEnabled(bIsToolEnabled);
}

ECheckBoxState FSkillEditorTool::IsToolEnabledChecked() const
{
    return bIsToolEnabled ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

bool FSkillEditorTool::IsToolEnabled() const
{
    return bIsToolEnabled;
}

TSharedRef<SComboBox<TSharedPtr<FName>>> FSkillEditorTool::CreateSkillComboBox(ESkillKey SkillKey)
{
    TSharedPtr<SComboBox<TSharedPtr<FName>>> ComboBox =
        SNew(SComboBox<TSharedPtr<FName>>)
        .OptionsSource(&SkillList)
        .IsEnabled(this, &FSkillEditorTool::IsToolEnabled) 
        .OnSelectionChanged(this, &FSkillEditorTool::OnSkillSelected, SkillKey)
        .OnGenerateWidget(this, &FSkillEditorTool::GenerateSkillComboBoxItem)
        [
            SNew(STextBlock)
            .Text(this, &FSkillEditorTool::GetSelectedSkillText, SkillKey) // 선택된 스킬 텍스트
        ];

    // 드롭다운을 TMap에 저장
    SkillDropdown.Add(SkillKey, ComboBox);
    return ComboBox.ToSharedRef();
}

void FSkillEditorTool::PopulateSkillDropdown()
{
    if (SkillDataTable)
    {
        // 데이터 테이블에서 행의 키 값(스킬 이름)들을 가져옴
        TArray<FName> RowNames = SkillDataTable->GetRowNames();
        SkillList.Empty();
        for (const FName& Name : RowNames)
        {
            SkillList.Add(MakeShared<FName>(Name));
        }
    }
}

void FSkillEditorTool::OnSkillSelected(TSharedPtr<FName> NewSelection, ESelectInfo::Type SelectInfo, ESkillKey SkillKey)
{
    if (SelectedSkills.Contains(SkillKey))
    {
        SelectedSkills[SkillKey] = NewSelection;
    }
    else
    {
        SelectedSkills.Add(SkillKey, NewSelection);
    }
}

TSharedRef<SWidget> FSkillEditorTool::GenerateSkillComboBoxItem(TSharedPtr<FName> InItem)
{
    return SNew(STextBlock).Text(FText::FromName(*InItem));
}

FText FSkillEditorTool::GetSelectedSkillText(ESkillKey SkillKey) const
{
    if (SelectedSkills.Contains(SkillKey))
    {
        return FText::FromName(*SelectedSkills[SkillKey]);
    }
    return FText::FromString("Select a skill");
}

FReply FSkillEditorTool::EquipSelectedSkill()
{
    UWorld* World = GetEditorWorld();

    if (!World)
    {
        return FReply::Unhandled();
    }
    
    APlayerController* PlayerController = World->GetFirstPlayerController();
    if (!PlayerController)
    {
        return FReply::Unhandled();
    }

    AArcanePunkCharacter* PlayerCharacter = Cast<AArcanePunkCharacter>(PlayerController->GetCharacter());
    if (!PlayerCharacter)
    {
        return FReply::Unhandled();
    }

    for(auto SkillTuple : SelectedSkills)
    {
        ESkillKey SkillKey = SkillTuple.Key;
        TSharedPtr<FName> SkillName = SkillTuple.Value;
        if(SkillName.IsValid())
        {
            PlayerCharacter->SetSkill(SkillKey, *SkillName.Get());
        }
    }

    return FReply::Handled();
}

UWorld* FSkillEditorTool::GetEditorWorld()
{
    UWorld* World = nullptr;

    for (const FWorldContext& Context : GEngine->GetWorldContexts())
    {
        if (Context.WorldType == EWorldType::PIE)
        {
            return Context.World();
        }
    }

    return World;
}