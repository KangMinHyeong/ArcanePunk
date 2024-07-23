
#include "UserInterface/EnhanceUI/ChoiceButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "UserInterface/EnhanceUI/APEnhanceChoice.h"
#include "Components/Character/SkillNumber/SkillDataTable/SkillDataTable.h"
#include "Character/ArcanePunkCharacter.h"
#include "GameInstance/APGameInstance.h"

void UChoiceButton::SetEnhance(UUserWidget* UpdateParentWidget, uint8 UpdateSkillNumber, uint8 UpdateSkillAbility ,uint16 UpdateNestingNumb)
{
    bEnhance = true;

    ParentWidget = Cast<UAPEnhanceChoice>(UpdateParentWidget);
    SkillNumber = UpdateSkillNumber;
    SkillAbility = UpdateSkillAbility;
    NestingNumb = UpdateNestingNumb;

    SetSkillName();
    auto DataTable = ParentWidget->GetSkillAbilityRowNameData()->FindRow<FSkillAbilityRowNameData>(SkillNumberName, SkillNumberName.ToString()); if(!DataTable) return;

    FSkillAbilityDataSheet* AbilityData = nullptr;
    TArray<FString> RowName;
    auto OwnerCharacter = Cast<AArcanePunkCharacter>(ParentWidget->GetOwningPlayerPawn()); if(!OwnerCharacter) return;
    auto APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI) return;
    switch (ParentWidget->GetEnHanceType())
    {
        case EEnHanceType::Silver:
        RowName = DataTable->SilverRowName;
        AbilityData = APGI->GetSilverAbilityDataTable()->FindRow<FSkillAbilityDataSheet>(FName(*RowName[UpdateSkillAbility]), RowName[UpdateSkillAbility]);
        break;

        case EEnHanceType::Gold:
        RowName = DataTable->GoldRowName;
        AbilityData = APGI->GetGoldAbilityDataTable()->FindRow<FSkillAbilityDataSheet>(FName(*RowName[UpdateSkillAbility]), RowName[UpdateSkillAbility]);
        break;

        case EEnHanceType::Platinum:
        RowName = DataTable->PlatinumRowName;
        AbilityData = APGI->GetPlatinumAbilityDataTable()->FindRow<FSkillAbilityDataSheet>(FName(*RowName[UpdateSkillAbility]), RowName[UpdateSkillAbility]);
        break;
    }
    if(!AbilityData) return;

    MaxNestingNum = AbilityData->MaxNesting;
    if(!AbilityData->Nestable) MaxNestingNum = 1;

    SkillNumber_Text->SetText(FText::FromString(AbilityData->AbilityName));  
    SkillAbility_Text->SetText(FText::FromString(AbilityData->AbilityInformation));
    SkillAbility_Nesting->SetText(FText::FromString(FString::FromInt(UpdateNestingNumb - 1)));
    SkillNumber_Image->SetBrushFromTexture(AbilityData->EnhanceSlotImage); 

    BindButton();
}

void UChoiceButton::SetNewSkill(UUserWidget* UpdateParentWidget, uint8 UpdateSkillNumber)
{
    bEnhance = false;
    SizeBox_Nesting->SetVisibility(ESlateVisibility::Hidden);
    ParentWidget = Cast<UAPEnhanceChoice>(UpdateParentWidget);
    SkillNumber = UpdateSkillNumber;

    SetSkillName();
    auto NameTable = SkillNameDataTable->FindRow<FSkillNameList>(SkillNumberName, SkillNumberName.ToString()); if(!NameTable){return;}
    SkillAbility_Text->SetText(FText::FromString(NameTable->Skill_Information));   
    SkillNumber_Text->SetText(FText::FromString(NameTable->SkillName_Korean));
    SkillNumber_Image->SetBrushFromTexture(NameTable->SkillSlotImage); 
    Text_New->SetVisibility(ESlateVisibility::Visible);

    BindButton();
}

void UChoiceButton::SetSkillName()
{
    if(!ParentWidget.IsValid()) return;
    if(ParentWidget->GetEnHanceCategory() == EEnhanceCategory::Enhance_Passive)
    {
        const UEnum* SkillNum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.EPassiveNumber"), true); if(!SkillNum) return;
        SkillNumberName = FName(*SkillNum->GetNameStringByValue(SkillNumber));
    }
    else
    {
        if(bEnhance)
        {
            SkillNumberName = ParentWidget->GetSkillAbilityNestingData().SkillName;
        }
        else
        {
            const UEnum* SkillNum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.ESkillNumber"), true); if(!SkillNum) return;
            SkillNumberName = FName(*SkillNum->GetNameStringByValue(SkillNumber));
        }
    }
}

void UChoiceButton::BindButton()
{   
    Choice_Button->OnClicked.AddDynamic(this, &UChoiceButton::OnClickChoice);
    Reroll_Button->OnClicked.AddDynamic(this, &UChoiceButton::OnReroll);

    Choice_Button->OnHovered.AddDynamic(this, &UChoiceButton::OnChoiceButton_Hovered);
    Choice_Button->OnUnhovered.AddDynamic(this, &UChoiceButton::OnChoiceButton_UnHovered);

    Reroll_Button->OnHovered.AddDynamic(this, &UChoiceButton::OnRerollButton_Hovered);
    Reroll_Button->OnUnhovered.AddDynamic(this, &UChoiceButton::OnRerollButton_UnHovered);
}

void UChoiceButton::OnClickChoice()
{
    Choice_Button->OnClicked.RemoveDynamic(this, &UChoiceButton::OnClickChoice);
    OnEnhanceChoice();
    if(ParentWidget.IsValid()) ParentWidget->OnBackGround_FadeOut();
}

void UChoiceButton::OnReroll()
{
    Reroll_Button->OnClicked.RemoveDynamic(this, &UChoiceButton::OnReroll);
    if(!ParentWidget.IsValid()) return;
    ParentWidget->OnReroll(ChoiceIndexNum);
    // SetChoiceButton();
}

void UChoiceButton::ApplyChoice()
{
    if(!ParentWidget.IsValid()) return;

    if(bEnhance)
    {
        ParentWidget->ApplyEnhance(SkillNumber, SkillAbility, NestingNumb, MaxNestingNum);
    }
    else
    {
        ParentWidget->ApplyNewSkill(SkillNumber);
    }
}
