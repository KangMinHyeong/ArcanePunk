
#include "UserInterface/EnhanceUI/ChoiceButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UserInterface/EnhanceUI/APEnhanceChoice.h"

// void UChoiceButton::SetEnhance(UUserWidget* UpdateParentWidget, ESkillNumber UpdateSkillNumber ,ESkillAbility UpdateSkillAbility)
// {
//     bEnhance = true;
    
//     ParentWidget = UpdateParentWidget;
//     SkillNumber = UpdateSkillNumber;
//     SkillAbility = UpdateSkillAbility;

//     const UEnum* SkillNum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.ESkillNumber"), true); if(!SkillNum) return;
//     SkillNumber_Text->SetText(FText::FromString(SkillNum->GetDisplayNameTextByIndex((uint8)UpdateSkillNumber).ToString()));

//     const UEnum* SkillAbil = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.ESkillAbility"), true); if(!SkillAbil) return;
//     SkillAbility_Text->SetText(FText::FromString(SkillAbil->GetDisplayNameTextByIndex((uint8)UpdateSkillAbility).ToString()));

//     BindButton();
// }

void UChoiceButton::SetEnhance(UUserWidget* UpdateParentWidget, ESkillNumber UpdateSkillNumber, uint8 UpdateSkillAbility ,uint16 UpdateNestingNumb)
{
    bEnhance = true;

    ParentWidget = Cast<UAPEnhanceChoice>(UpdateParentWidget);
    SkillNumber = UpdateSkillNumber;
    SkillAbility = UpdateSkillAbility;
    NestingNumb = UpdateNestingNumb;

    const UEnum* SkillNum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.ESkillNumber"), true); if(!SkillNum) return;
    FName SkillNumberName = ParentWidget->GetSkillAbilityNestingData().SkillName;
    
    auto DataTable = ParentWidget->GetSkillAbilityDataTable()->FindRow<FSkillAbilityDataTable>(SkillNumberName, SkillNumberName.ToString()); if(!DataTable) return;

    if(ParentWidget->GetEnHanceType() == EEnHanceType::Silver) {SkillNumber_Text->SetText(FText::FromString(DataTable->SilverAbilityInformation[SkillAbility].AbilityName));  SkillAbility_Text->SetText(FText::FromString(DataTable->SilverAbilityInformation[SkillAbility].AbilityInformation));}
    else if(ParentWidget->GetEnHanceType() == EEnHanceType::Gold) {SkillNumber_Text->SetText(FText::FromString(DataTable->GoldAbilityInformation[SkillAbility].AbilityName));  SkillAbility_Text->SetText(FText::FromString(DataTable->GoldAbilityInformation[SkillAbility].AbilityInformation));}
    else {SkillNumber_Text->SetText(FText::FromString(DataTable->PlatinumAbilityInformation[SkillAbility].AbilityName)); SkillAbility_Text->SetText(FText::FromString(DataTable->PlatinumAbilityInformation[SkillAbility].AbilityInformation));}

    BindButton();
}

void UChoiceButton::SetNewSkill(UUserWidget* UpdateParentWidget, ESkillNumber UpdateSkillNumber)
{
    bEnhance = false;
    
    ParentWidget = Cast<UAPEnhanceChoice>(UpdateParentWidget);
    SkillNumber = UpdateSkillNumber;

    FName NewSkill_TEXT = TEXT("New!");
    SkillNumber_Text->SetText(FText::FromName(NewSkill_TEXT));

    const UEnum* SkillNum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.ESkillNumber"), true); if(!SkillNum) return;
    SkillAbility_Text->SetText(FText::FromString(SkillNum->GetDisplayNameTextByIndex((uint8)UpdateSkillNumber).ToString()));

    BindButton();
}

void UChoiceButton::BindButton()
{   
    Choice_Button->OnClicked.AddDynamic(this, &UChoiceButton::OnEnhanceChoice);
}

void UChoiceButton::OnEnhanceChoice()
{
    if(!ParentWidget.IsValid()) return;

    if(bEnhance)
    {
        ParentWidget->ApplyEnhance(SkillAbility, NestingNumb);
    }
    else
    {
        ParentWidget->ApplyNewSkill(SkillNumber);
    }
}
