
#include "UserInterface/Inform/Skill/APSkillWindowInformation.h"

#include "Character/ArcanePunkCharacter.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/StackBox.h"
#include "Components/SizeBox.h"
#include "GameInstance/APGameInstance.h"
#include "UserInterface/Inform/Skill/APSkillWindow.h"

void UAPSkillWindowInformation::NativeConstruct()
{
    Super::NativeConstruct();
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn()); if(!OwnerCharacter.IsValid()) return;
    APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI.IsValid()) return;

}

void UAPSkillWindowInformation::InitInformationData_Skill(EEnhanceCategory UpdateEnhanceCategory, FSkillNameList* SkillNameTable, float CoolTime)
{
    FString CategoryTypeName;
    switch (UpdateEnhanceCategory)
    {
    case EEnhanceCategory::Enhance_Q:
        CategoryTypeName = TEXT("타입 : Q 스킬");
        break;
    
    case EEnhanceCategory::Enhance_E:
        CategoryTypeName = TEXT("타입 : E 스킬");
        break;

    case EEnhanceCategory::Enhance_R:
        CategoryTypeName = TEXT("타입 : R 스킬");
        break;

    case EEnhanceCategory::Enhance_Passive:
        CategoryTypeName = TEXT("타입 : 패시브");
        break;
    }

    Image_Skill->SetBrushFromTexture(SkillNameTable->SkillSlotImage); 
    Text_Name->SetText(FText::FromString(SkillNameTable->SkillName_Korean));
    Text_Category->SetText(FText::FromString(CategoryTypeName));
    Text_Information->SetText(FText::FromString(SkillNameTable->Skill_Information));

    if(UpdateEnhanceCategory == EEnhanceCategory::Enhance_Passive) 
    {
        StackBox_Information->RemoveChild(SizeBox_Ability_1);
        StackBox_Information->RemoveChild(SizeBox_Ability_2);
    }
    else
    {
        Text_Ability->SetText(FText::FromString(CurrentAbility));
        Text_AbilityNum_End->SetText(FText::FromString(EndString_2));
        Text_Ability_2->SetText(FText::FromString(CurrentAbility_2));
        Text_AbilityNum_2->SetText(FText::FromString(FString::FromInt(SkillNameTable->MPConsumption)));
        Text_AbilityNum_End_2->SetText(FText::FromString(EndString_3));
        DisplayCoolTime(CoolTime);
    }
}

void UAPSkillWindowInformation::DisplayCoolTime(float CoolTime)
{
    if(CoolTime - floor(CoolTime) <= KINDA_SMALL_NUMBER)
    {
        Text_AbilityNum->SetText(FText::FromString(FString::FromInt(round(CoolTime))));
    }
    else
    {
        Text_AbilityNum->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), CoolTime)));
    }
}

void UAPSkillWindowInformation::InitInformationData_Enhance(EEnHanceType UpdateEnhanceType, FSkillAbilityDataSheet* UpdateAbilityData, uint16 UpdateNestingNumber)
{
    FString CategoryTypeName;
    switch (UpdateEnhanceType)
    {
    case EEnHanceType::Silver:
        Image_TierColor->SetColorAndOpacity(SilverColor);
        CategoryTypeName = TEXT("티어 : 실버");
        break;
    
    case EEnHanceType::Gold:
        Image_TierColor->SetColorAndOpacity(GoldColor);
        CategoryTypeName = TEXT("티어 : 골드");
        break;

    case EEnHanceType::Platinum:
        Image_TierColor->SetColorAndOpacity(PlatinumColor);
        CategoryTypeName = TEXT("티어 : 플레티넘");
        break;
    }

    Image_Skill->SetBrushFromTexture(UpdateAbilityData->EnhanceSlotImage); 
    Text_Name->SetText(FText::FromString(UpdateAbilityData->AbilityName));
    Text_Category->SetText(FText::FromString(CategoryTypeName));
    Text_Information->SetText(FText::FromString(UpdateAbilityData->AbilityInformation));
    Text_Ability->SetText(FText::FromString(CurrentNesting));
    Text_AbilityNum->SetText(FText::FromString(FString::FromInt(UpdateNestingNumber)));
    Text_AbilityNum_End->SetText(FText::FromString(EndString_1));
    Text_Ability_2->SetText(FText::FromString(MaxNesting));
    Text_AbilityNum_2->SetText(FText::FromString(FString::FromInt(UpdateAbilityData->MaxNesting)));
    Text_AbilityNum_End_2->SetText(FText::FromString(EndString_1));
}
