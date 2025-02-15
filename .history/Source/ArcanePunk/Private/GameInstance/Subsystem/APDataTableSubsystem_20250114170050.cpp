// Fill out your copyright notice in the Description page of Project Settings.

#include "GameInstance/Subsystem/APDataTableSubsystem.h"

#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "GameElements/Drop/APManaEnergy.h"
#include "Enemy/Drop/Enemy_DropBase.h"
#include "UserInterface/Common/DisplayOnly/APSystemMessage.h"

UAPDataTableSubsystem::UAPDataTableSubsystem()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> SkillNameListData(TEXT("/Game/DataTable/Skill/SkillListData"));
	if(SkillNameListData.Succeeded()) SkillNameListDataTable = SkillNameListData.Object;

    static ConstructorHelpers::FObjectFinder<UDataTable> SkillAbilityRowData(TEXT("/Game/DataTable/Skill/SkillAbilityRowNameData"));
	if(SkillAbilityRowData.Succeeded()) SkillAbilityRowDataTable = SkillAbilityRowData.Object;

    static ConstructorHelpers::FObjectFinder<UDataTable> SilverAbilityData(TEXT("/Game/DataTable/Skill/ArcanePunk_SkillAbilityData_Silver"));
	if(SilverAbilityData.Succeeded()) SilverAbilityDataTable = SilverAbilityData.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> GoldAbilityData(TEXT("/Game/DataTable/Skill/ArcanePunk_SkillAbilityData_Gold"));
	if(GoldAbilityData.Succeeded()) GoldAbilityDataTable = GoldAbilityData.Object;

    static ConstructorHelpers::FObjectFinder<UDataTable> PlatinumAbilityData(TEXT("/Game/DataTable/Skill/ArcanePunk_SkillAbilityData_Platinum"));
	if(PlatinumAbilityData.Succeeded()) PlatinumAbilityDataTable = PlatinumAbilityData.Object;

    static ConstructorHelpers::FObjectFinder<UDataTable> StringData(TEXT("/Game/DataTable/String/StringData"));
	if(StringData.Succeeded()) StringDataTable = StringData.Object;

    static ConstructorHelpers::FObjectFinder<UDataTable> DialogueData(TEXT("/Game/DataTable/String/DialogueData"));
	if(DialogueData.Succeeded()) DialogueDataTable = DialogueData.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterData(TEXT("/Game/DataTable/Character/CharacterData"));
	if(CharacterData.Succeeded()) CharacterDataTable = CharacterData.Object;

    static ConstructorHelpers::FObjectFinder<UDataTable> DropData(TEXT("/Game/DataTable/Character/DropData"));
	if(DropData.Succeeded()) DropDataTable = DropData.Object;

    static ConstructorHelpers::FObjectFinder<UDataTable> CharacterUIData(TEXT("/Game/DataTable/Character/CharacterUIData"));
	if(CharacterUIData.Succeeded()) CharacterUIDataTable = CharacterUIData.Object;
    
    static ConstructorHelpers::FObjectFinder<UDataTable> EquipData(TEXT("/Game/DataTable/Character/ItemData"));
	if(EquipData.Succeeded()) EquipDataTable = EquipData.Object;

    // BP_Class
    static ConstructorHelpers::FClassFinder<AAPManaEnergy> ManaEnergy(TEXT("/Game/Blueprints/GameElement/Drop/BP_APManaEnergy"));
	if(ManaEnergy.Succeeded()) ManaEnergyClass = ManaEnergy.Class;

    static ConstructorHelpers::FClassFinder<AEnemy_DropBase> DropGold(TEXT("/Game/Blueprints/Enemy/Drop/BP_DropGold"));
	if(DropGold.Succeeded()) DropGoldClass = DropGold.Class;

    static ConstructorHelpers::FClassFinder<UUserWidget> CheckUI(TEXT("/Game/Blueprints/UserInterfaces/UI/Common/Check/WBP_APCheckUI"));
	if(CheckUI.Succeeded()) CheckUIClass = CheckUI.Class;

    static ConstructorHelpers::FClassFinder<UUserWidget> SystemMessage(TEXT("/Game/Blueprints/UserInterfaces/UI/Common/System_Message/WBP_SystemMessage"));
	if(SystemMessage.Succeeded()) SystemMessageClass = SystemMessage.Class;
}

void UAPDataTableSubsystem::Initialize(FSubsystemCollectionBase &Collection)
{
    Super::Initialize(Collection);
    CheckEnum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.EStringRowName"));
    InitDialogueData();
}

void UAPDataTableSubsystem::DisplaySystemMesseage(UAPDataTableSubsystem* DataTableGI, EStringRowName StringRowName, bool bLowMessage, bool bWarning)
{
    if(!DataTableGI || !DataTableGI->GetSystemMessageClass()) return;

    auto SystemMessageUI = CreateWidget<UAPSystemMessage>(DataTableGI->GetWorld(), DataTableGI->GetSystemMessageClass()); if(!SystemMessageUI) return;
    SystemMessageUI->AddToViewport(500);
    SystemMessageUI->DisplaySystemMesseage(StringRowName, bLowMessage, bWarning);
}

void UAPDataTableSubsystem::SetTextBlock(UAPDataTableSubsystem* DataTableGI, UTextBlock *TextBlock, const EStringRowName & RowName)
{
    if(!DataTableGI) return;

    if(!DataTableGI->GetCheckEnum()) return;
    FString Name = DataTableGI->GetCheckEnum()->GetNameStringByValue((uint8)RowName);

    auto DataTable = DataTableGI->GetStringDataTable()->FindRow<FStringDataTable>(FName(*Name), Name); 
    if(DataTable) TextBlock->SetText(FText::FromString(DataTable->Content));    
}

void UAPDataTableSubsystem::SetTextBlock_Name(UAPDataTableSubsystem* DataTableGI, UTextBlock *TextBlock, const FName & RowName)
{
    if(!DataTableGI) return;

    auto DataTable = DataTableGI->GetStringDataTable()->FindRow<FStringDataTable>(RowName, RowName.ToString()); 
    if(DataTable) TextBlock->SetText(FText::FromString(DataTable->Content));  
}

const FString &UAPDataTableSubsystem::GetStringContent(const EStringRowName &RowName)
{
    FString Name = "";
    if(!CheckEnum) CheckEnum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.EStringRowName"));
    if(!CheckEnum) return Name;
    Name = CheckEnum->GetNameStringByValue((uint8)RowName);

    auto DataTable = StringDataTable->FindRow<FStringDataTable>(FName(*Name), Name); 
    return DataTable->Content;
}

void UAPDataTableSubsystem::InitDialogueData()
{
    if(!DialogueDataTable) return;

    GroupedDialogueRows.Empty();

    for(const FName& RowName : DialogueDataTable->GetRowNames())
    {
        FDialogueDataTable* Row = DialogueDataTable->FindRow<FDialogueDataTable>(RowName, TEXT("InitializeDialogueData"));
        if(Row)
        {
            // 그룹 ID를 기반으로 TMap에 추가
            if(GroupedDialogueRows.Contains(Row->Diologue_ID))
            {
                GroupedDialogueRows[Row->Diologue_ID].DialogueGroupData.Emplace(*Row);
            }
            else
            {
                FDialogueGroupData NewGroup;
                NewGroup.DialogueGroupData.Emplace(*Row);
                GroupedDialogueRows.Add({Row->Diologue_ID, NewGroup});
            }
        }
    }
}

const TArray<FDialogueDataTable> UAPDataTableSubsystem::GetDialogues(const int32 GroupID) const
{
    if(GroupedDialogueRows.Contains(GroupID))
    {
        return GroupedDialogueRows[GroupID].DialogueGroupData;
    }
    
    return TArray<FDialogueDataTable>();
}