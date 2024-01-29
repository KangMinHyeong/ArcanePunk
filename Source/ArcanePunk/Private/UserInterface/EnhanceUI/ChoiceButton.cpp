
#include "UserInterface/EnhanceUI/ChoiceButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UserInterface/EnhanceUI/APEnhanceChoice.h"

void UChoiceButton::SetEnhance(UUserWidget* UpdateParentWidget, ESkillNumber UpdateSkillNumber ,ESkillAbility UpdateSkillAbility)
{
    bEnhance = true;
    
    ParentWidget = UpdateParentWidget;
    SkillNumber = UpdateSkillNumber;
    SkillAbility = UpdateSkillAbility;

    const UEnum* SkillNum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.ESkillNumber"), true); if(!SkillNum) return;
    SkillNumber_Text->SetText(FText::FromString(SkillNum->GetDisplayNameTextByIndex((uint8)UpdateSkillNumber).ToString()));

    const UEnum* SkillAbil = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.ESkillAbility"), true); if(!SkillAbil) return;
    SkillAbility_Text->SetText(FText::FromString(SkillAbil->GetDisplayNameTextByIndex((uint8)UpdateSkillAbility).ToString()));

    BindButton();
}

void UChoiceButton::SetNewSkill(UUserWidget* UpdateParentWidget, ESkillNumber UpdateSkillNumber)
{
    bEnhance = false;
    
    ParentWidget = UpdateParentWidget;
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
    auto EC = Cast<UAPEnhanceChoice>(ParentWidget); if(!EC) return;

    if(bEnhance)
    {
        EC->ApplyEnhance(SkillAbility);
    }
    else
    {
        EC->ApplyNewSkill(SkillNumber);
    }
}
