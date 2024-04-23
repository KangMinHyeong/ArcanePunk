
#include "UserInterface/EnhanceUI/ChoiceButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "UserInterface/EnhanceUI/APEnhanceChoice.h"
#include "Components/Character/SkillNumber/SkillDataTable/SkillDataTable.h"
#include "Character/ArcanePunkCharacter.h"

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
    
    auto DataTable = ParentWidget->GetSkillAbilityRowNameData()->FindRow<FSkillAbilityRowNameData>(SkillNumberName, SkillNumberName.ToString()); if(!DataTable) return;
    auto SlotTable = SkillSlotDataTable->FindRow<FSkillSlotDataTable>(Name, Name.ToString()); if(!SlotTable){return;}
    
    SkillNumber_Image->SetBrushFromTexture(SlotTable->SkillSlotImage[(uint8)UpdateSkillNumber]); 

    FSkillAbilityDataSheet* AbilityData = nullptr;
    TArray<FString> RowName;
    auto OwnerCharacter = Cast<AArcanePunkCharacter>(ParentWidget->GetOwningPlayerPawn()); if(!OwnerCharacter) return;
    switch (ParentWidget->GetEnHanceType())
    {
        case EEnHanceType::Silver:
        RowName = DataTable->SilverRowName;
        AbilityData = OwnerCharacter->GetSilverAbilityDataTable()->FindRow<FSkillAbilityDataSheet>(FName(*RowName[UpdateSkillAbility]), RowName[UpdateSkillAbility]);
        break;

        case EEnHanceType::Gold:
        RowName = DataTable->GoldRowName;
        AbilityData = OwnerCharacter->GetGoldAbilityDataTable()->FindRow<FSkillAbilityDataSheet>(FName(*RowName[UpdateSkillAbility]), RowName[UpdateSkillAbility]);
        break;

        case EEnHanceType::Platinum:
        RowName = DataTable->PlatinumRowName;
        AbilityData = OwnerCharacter->GetPlatinumAbilityDataTable()->FindRow<FSkillAbilityDataSheet>(FName(*RowName[UpdateSkillAbility]), RowName[UpdateSkillAbility]);
        break;
    }
    if(!AbilityData) return;

    MaxNestingNum = AbilityData->MaxNesting;
    if(!AbilityData->Nestable) MaxNestingNum = 1;

    SkillNumber_Text->SetText(FText::FromString(AbilityData->AbilityName));  
    SkillAbility_Text->SetText(FText::FromString(AbilityData->AbilityInformation));

    // if(ParentWidget->GetEnHanceType() == EEnHanceType::Silver) {SkillNumber_Text->SetText(FText::FromString(DataTable->SilverAbilityInformation[SkillAbility].AbilityName));  SkillAbility_Text->SetText(FText::FromString(DataTable->SilverAbilityInformation[SkillAbility].AbilityInformation));}
    // else if(ParentWidget->GetEnHanceType() == EEnHanceType::Gold) {SkillNumber_Text->SetText(FText::FromString(DataTable->GoldAbilityInformation[SkillAbility].AbilityName));  SkillAbility_Text->SetText(FText::FromString(DataTable->GoldAbilityInformation[SkillAbility].AbilityInformation));}
    // else {SkillNumber_Text->SetText(FText::FromString(DataTable->PlatinumAbilityInformation[SkillAbility].AbilityName)); SkillAbility_Text->SetText(FText::FromString(DataTable->PlatinumAbilityInformation[SkillAbility].AbilityInformation));}

    SkillAbility_Nesting->SetText(FText::FromString(FString::FromInt(UpdateNestingNumb - 1)));
    
    BindButton();
}

void UChoiceButton::SetNewSkill(UUserWidget* UpdateParentWidget, ESkillNumber UpdateSkillNumber)
{
    bEnhance = false;
    
    ParentWidget = Cast<UAPEnhanceChoice>(UpdateParentWidget);
    SkillNumber = UpdateSkillNumber;

    const UEnum* SkillNum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.ESkillNumber"), true); if(!SkillNum) return;
    SkillNumber_Text->SetText(FText::FromString(SkillNum->GetDisplayNameTextByIndex((uint8)UpdateSkillNumber).ToString()));

    FString NewSkill_TEXT = TEXT("스킬 설명 추가");
    SkillAbility_Text->SetText(FText::FromString(NewSkill_TEXT));

    auto SlotTable = SkillSlotDataTable->FindRow<FSkillSlotDataTable>(Name, Name.ToString()); if(!SlotTable){return;}
    
    SkillNumber_Image->SetBrushFromTexture(SlotTable->SkillSlotImage[(uint8)UpdateSkillNumber]); 

    BindButton();
}

void UChoiceButton::BindButton()
{   
    Choice_Button->OnClicked.AddDynamic(this, &UChoiceButton::OnEnhanceChoice);
    Reroll_Button->OnClicked.AddDynamic(this, &UChoiceButton::OnReroll);
}

void UChoiceButton::OnEnhanceChoice()
{
    if(!ParentWidget.IsValid()) return;

    if(bEnhance)
    {
        ParentWidget->ApplyEnhance(SkillAbility, NestingNumb, MaxNestingNum);
    }
    else
    {
        ParentWidget->ApplyNewSkill(SkillNumber);
    }
}

void UChoiceButton::OnReroll()
{
    if(!ParentWidget.IsValid()) return;
    ParentWidget->OnReroll(ChoiceIndexNum);
    // SetChoiceButton();

}

