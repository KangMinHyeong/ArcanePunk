
#include "GameInstance/APGameInstance.h"

void UAPGameInstance::Init()
{
    Super::Init();
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
