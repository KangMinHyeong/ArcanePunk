
#include "UserInterface/Status/HUD/APSkillSlot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UAPSkillSlot::NativeConstruct()
{
    Super::NativeConstruct();
    Text_SkillKey->SetText(FText::FromString(SkillKey));

}

void UAPSkillSlot::UpdateSkillSlot(uint8 SkillNumber)
{
    auto DataTable = SkillSlotDataTable->FindRow<FSkillSlotDataTable>(Name, Name.ToString()); if(!DataTable){return;}

    SkillImage->SetBrushFromTexture(DataTable->SkillSlotImage[SkillNumber]); 
}

void UAPSkillSlot::SetUsingSkill(bool NewBool)
{
    if(NewBool)
    {
        SkillUsingImage->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        SkillUsingImage->SetVisibility(ESlateVisibility::Hidden);
    }
}
