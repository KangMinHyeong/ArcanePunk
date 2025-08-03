// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcanePunk/Public/GameInstance/Subsystem/APDataTableSubsystem.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "DataStructs/Common/FLocalizationData.h"
#include "GameElements/Drop/APManaEnergy.h"
#include "Enemy/Drop/Enemy_DropBase.h"
#include "UserInterface/Common/DisplayOnly/APSystemMessage.h"

UAPDataTableSubsystem::UAPDataTableSubsystem()
{
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

void UAPDataTableSubsystem::CollectDataTablesByStruct()
{
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    TArray<FAssetData> DataTableAssets;
    FName DataTablePath(TEXT("/Game/DataTable"));
    AssetRegistryModule.Get().GetAssetsByPath(DataTablePath, DataTableAssets, true);
    FTopLevelAssetPath DataTableClassPath(TEXT("/Script/Engine"), TEXT("DataTable"));

    AllDataTablesByStruct.Empty();

    for (const FAssetData& Asset : DataTableAssets)
    {
        if (Asset.AssetClassPath == DataTableClassPath)
        {
            UDataTable* DataTable = Cast<UDataTable>(Asset.GetAsset());
            if (DataTable && DataTable->GetRowStruct())
            {
                const UScriptStruct* RowStruct = DataTable->GetRowStruct();
                AllDataTablesByStruct.Add(RowStruct, DataTable);
            }
        }
    }
}

void UAPDataTableSubsystem::Initialize(FSubsystemCollectionBase &Collection)
{
    Super::Initialize(Collection);

    CollectDataTablesByStruct();

    CheckEnum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.EStringRowName"));
}

void UAPDataTableSubsystem::DisplaySystemMesseage(UAPDataTableSubsystem* DataTableGI, EStringRowName StringRowName, bool bLowMessage, bool bWarning)
{
    if(!DataTableGI || !DataTableGI->GetSystemMessageClass()) return;

    auto SystemMessageUI = CreateWidget<UAPSystemMessage>(DataTableGI->GetWorld(), DataTableGI->GetSystemMessageClass()); if(!SystemMessageUI) return;
    SystemMessageUI->AddToViewport(500);
    //SystemMessageUI->DisplaySystemMesseage(StringRowName, bLowMessage, bWarning);
}

void UAPDataTableSubsystem::DisplaySystemMesseage(UAPDataTableSubsystem *DataTableGI, const FName &Message, bool bLowMessage, bool bWarning)
{
    if(!DataTableGI || !DataTableGI->GetSystemMessageClass()) return;
    auto SystemMessageUI = CreateWidget<UAPSystemMessage>(DataTableGI->GetWorld(), DataTableGI->GetSystemMessageClass()); if(!SystemMessageUI) return;
    SystemMessageUI->AddToViewport(500);
    //SystemMessageUI->DisplaySystemMesseage(Message, bLowMessage, bWarning);
}

void UAPDataTableSubsystem::SetTextBlock(UAPDataTableSubsystem* DataTableGI, UTextBlock *TextBlock, const EStringRowName & RowName)
{
    if(!DataTableGI) return;

    if(!DataTableGI->GetCheckEnum()) return;
    FString Name = DataTableGI->GetCheckEnum()->GetNameStringByValue((uint8)RowName);

    auto DataTable = DataTableGI->GetRowByStruct<FLocalizationData>(FName(*Name), Name); 
    if(DataTable) TextBlock->SetText(FText::FromString(DataTable->Content));    
}

void UAPDataTableSubsystem::SetTextBlock_Name(UAPDataTableSubsystem* DataTableGI, UTextBlock *TextBlock, const FName & RowName)
{
    if(!DataTableGI) return;

    auto DataTable = DataTableGI->GetRowByStruct<FLocalizationData>(RowName, RowName.ToString()); 
    if(DataTable) TextBlock->SetText(FText::FromString(DataTable->Content));  
}

//@오류 발생
//const FString &UAPDataTableSubsystem::GetStringContent(const EStringRowName &RowName)
//{
//    FString Name = "";
//    if(!CheckEnum) CheckEnum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.EStringRowName"));
//    if(!CheckEnum) return Name;
//    Name = CheckEnum->GetNameStringByValue((uint8)RowName);
//
//    auto DataTable = StringDataTable->FindRow<FStringDataTable>(FName(*Name), Name); 
//    return DataTable->Content;
//}

//@수정 버전
const FString& UAPDataTableSubsystem::GetStringContent(const EStringRowName& RowName)
{
    static FString EmptyString = "";
    if (!CheckEnum) CheckEnum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.EStringRowName"));
    if (!CheckEnum) return EmptyString;

    static FString Name;
    Name = CheckEnum->GetNameStringByValue((uint8)RowName);

    auto DataTable = this->GetRowByStruct<FLocalizationData>(FName(*Name), Name);
    if (!DataTable) return EmptyString;

    return DataTable->Content;
}

const TArray<FDialogueData> UAPDataTableSubsystem::GetDialogues(const int32 GroupID) const
{
    if(GroupedDialogueRows.Contains(GroupID))
    {
        return GroupedDialogueRows[GroupID].DialogueGroupData;
    }
    
    return TArray<FDialogueData>();
}