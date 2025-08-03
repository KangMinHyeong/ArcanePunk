#include "UserInterface/HUD/Status/Bar/ImitatorSkillSlot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APSkillHubComponent.h"
#include "DataStructs/Skill/FSkillNameList.h"

void UImitatorSkillSlot::NativeConstruct()
{
    SkillImage->SetVisibility(ESlateVisibility::Hidden);
    OnSkillAction_FadeIn();
}

void UImitatorSkillSlot::UpdateSkillSlot(uint8 SkillNumber)
{
    SkillImage->SetVisibility(ESlateVisibility::Visible);
    if(SkillNumber == 0) {OnSkillAction_FadeOut(); return;}
    OnSkillAction_FadeIn(); 

    const UEnum* SkillNum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.ESkillNumber"), true); if(!SkillNum) return;
    FString SkillName = SkillNum->GetNameStringByValue(SkillNumber);
    auto NameTable = SkillNameData->FindRow<FSkillNameList>(FName(*SkillName), SkillName); if(!NameTable) return;
    Text_SkillName->SetText(FText::FromString(NameTable->SkillName_Korean));
    SkillImage->SetBrushFromTexture(NameTable->SkillSlotImage);
}

