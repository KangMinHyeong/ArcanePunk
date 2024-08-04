
#include "GameInstance/APGameInstance.h"

#include "Save/APSaveGame.h"
#include "Components/TextBlock.h"

void UAPGameInstance::Init()
{
    Super::Init();

    CheckEnum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.EStringRowName"), true);
    APSaveGameData = NewObject<UAPSaveGame>();

    OnSkillEnhanceDataUpdate.AddUObject(this, &UAPGameInstance::UpdateSkillEnhanceData);
    OnSkillEnhanceDataClear.AddUObject(this, &UAPGameInstance::ClearSkillEnhanceData);
}

void UAPGameInstance::UpdateSkillEnhanceData(ESkillKey UpdateSkillKey, FSkillAbilityNestingData UpdateSkillAbilityNestingData)
{
    switch (UpdateSkillKey)
    {
        case ESkillKey::Q:
        QSkillAbilityNestingData = UpdateSkillAbilityNestingData;
        break;

        case ESkillKey::E:
        ESkillAbilityNestingData = UpdateSkillAbilityNestingData;
        break;

        case ESkillKey::R:
        RSkillAbilityNestingData = UpdateSkillAbilityNestingData;
        break;
    }
}

void UAPGameInstance::ClearSkillEnhanceData(ESkillKey UpdateSkillKey)
{
    switch (UpdateSkillKey)
    {
        case ESkillKey::Q:
        QSkillAbilityNestingData = {};
        break;

        case ESkillKey::E:
        ESkillAbilityNestingData = {};
        break;

        case ESkillKey::R:
        RSkillAbilityNestingData = {};
        break;
    }
}

int32 UAPGameInstance::CheckGoldAmount()
{
    int32 Amount = 1;
    float PlusPercent = FMath::RandRange(0.0f, 100.0f);
    if(PlusPercent <= GoldPlusPercent) Amount++;
    OnGettingGold.Broadcast(Amount);

    return Amount;
}

void UAPGameInstance::SetTextBlock(UTextBlock *TextBlock, EStringRowName RowName)
{
    if(!CheckEnum.IsValid()) return;
    FString Name = CheckEnum->GetNameStringByValue((uint8)RowName);

    auto DataTable = StringData->FindRow<FStringDataTable>(FName(*Name), Name); 
    if(DataTable) TextBlock->SetText(FText::FromString(DataTable->Content));    
}

void UAPGameInstance::SetTextBlock_Name(UTextBlock *TextBlock, FName RowName)
{
    auto DataTable = StringData->FindRow<FStringDataTable>(RowName, RowName.ToString()); 
    if(DataTable) TextBlock->SetText(FText::FromString(DataTable->Content));  
}

FString UAPGameInstance::GetStringContent(EStringRowName RowName)
{
    if(!CheckEnum.IsValid()) return "";
    FString Name = CheckEnum->GetNameStringByValue((uint8)RowName);

    auto DataTable = StringData->FindRow<FStringDataTable>(FName(*Name), Name); 
    return DataTable->Content;
}
