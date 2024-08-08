
#include "UserInterface/Inform/Skill/APEnhanceSlot.h"

#include "Character/ArcanePunkCharacter.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "GameInstance/APGameInstance.h"
#include "UserInterface/Inform/Skill/APSkillWindow.h"

void UAPEnhanceSlot::NativeConstruct()
{
    Super::NativeConstruct();
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn()); if(!OwnerCharacter.IsValid()) return;
    APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI.IsValid()) return;

    Button_Enhance->OnClicked.AddDynamic(this, &UAPEnhanceSlot::OnEnhanceInformation);
}

void UAPEnhanceSlot::InitEnhanceData(UUserWidget *Parent, EEnHanceType UpdateEnhanceType, FName UpdateRowName, uint8 UpdateEnhanceNumber, uint16 UpdateNestingNumber)
{
    ParentWidget = Parent;
    EnHanceType = UpdateEnhanceType;
    EnhanceNumber = UpdateEnhanceNumber;
    NestingNumber = UpdateNestingNumber;

    auto DataTable = APGI->GetSkillAbilityRowData()->FindRow<FSkillAbilityRowNameData>(UpdateRowName, UpdateRowName.ToString()); if(!DataTable) return;
    switch (EnHanceType)
    {
    case EEnHanceType::Silver:
        RowName = DataTable->SilverRowName[EnhanceNumber-1];
        AbilityData = APGI->GetSilverAbilityDataTable()->FindRow<FSkillAbilityDataSheet>(FName(*RowName), RowName); 
        break;
    
    case EEnHanceType::Gold:
        RowName = DataTable->GoldRowName[EnhanceNumber-1];
        AbilityData = APGI->GetGoldAbilityDataTable()->FindRow<FSkillAbilityDataSheet>(FName(*RowName), RowName); 
        Image_TierColor->SetColorAndOpacity(GoldColor);
        break;

    case EEnHanceType::Platinum:
        RowName = DataTable->PlatinumRowName[EnhanceNumber-1];
        AbilityData = APGI->GetPlatinumAbilityDataTable()->FindRow<FSkillAbilityDataSheet>(FName(*RowName), RowName); 
        Image_TierColor->SetColorAndOpacity(PlatinumColor);
        break;
    }
    if(!AbilityData) return;
    auto EnhanceImage = AbilityData->EnhanceSlotImage;

    auto ButtonStyle = Button_Enhance->GetStyle(); 
    ButtonStyle.Normal.SetResourceObject(EnhanceImage);
    ButtonStyle.Hovered.SetResourceObject(EnhanceImage);
    ButtonStyle.Pressed.SetResourceObject(EnhanceImage);
    Button_Enhance->SetStyle(ButtonStyle); 
    // RowName 바꾸기 Skill_8 -> Skill_8_1
    // image 바꾸기
}

void UAPEnhanceSlot::OnEnhanceInformation()
{
    auto SkillWindow = Cast<UAPSkillWindow>(ParentWidget.Get()); if(!SkillWindow) return;
    SkillWindow->OpenEnhanceInformation(EnHanceType, AbilityData, NestingNumber);
}