
#include "UserInterface/EnhanceUI/APEnhanceChoice.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Items/APItemBase.h"
#include "ArcanePunk/Public/Components/APInventoryComponent.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Components/WrapBox.h"
#include "UserInterface/EnhanceUI/ChoiceButton.h"

void UAPEnhanceChoice::NativeConstruct()
{
    Super::NativeConstruct();

    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn()); if(!OwnerCharacter) return;

    // InitSuffle();
    SetPauseGame();
}

void UAPEnhanceChoice::SetPauseGame()
{
    if(!OwnerCharacter) return;
    auto PC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!PC) return;

    PC->SetPause(true);
}

void UAPEnhanceChoice::TextSetting()
{
    if(!OwnerCharacter) return;
    
    if(EnHanceType == EEnHanceType::Silver) {EnHanceType_Text->SetText(FText::FromName(TEXT_Silver));}
    else if(EnHanceType == EEnHanceType::Gold) {EnHanceType_Text->SetText(FText::FromName(TEXT_Gold));}
    else if(EnHanceType == EEnHanceType::Platinum) {EnHanceType_Text->SetText(FText::FromName(TEXT_Platinum));}
    
    
    auto Item = OwnerCharacter->GetInventory()->FindItembyId(TEXT("test_005")); 
    if(!Item){ RerollDice_Text->SetText(FText::FromString(FString::FromInt(0)));}
    else { RerollDice_Text->SetText(FText::FromString(FString::FromInt(Item->Quantity)));}
}

void UAPEnhanceChoice::InitType(EEnhanceCategory UpdateEnhanceCategory, EEnHanceType UpdateEnHanceType)
{
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn()); if(!OwnerCharacter) return;
    EnhanceCategory = UpdateEnhanceCategory;
    EnHanceType = UpdateEnHanceType;

    NewSkills.Empty(); SkillAbilities.Empty();

    SetAbility();
    SetChoiceButton();
    TextSetting();
    Reroll_Button->OnClicked.AddDynamic(this, &UAPEnhanceChoice::OnReroll);
}

void UAPEnhanceChoice::SetAbility()
{
    if(!OwnerCharacter) return;

    switch (EnhanceCategory)
    {
        case EEnhanceCategory::Enhance_Q:
        SkillNumber = OwnerCharacter->GetQSkill();
        OriginSkillAbilityList = OwnerCharacter->GetAbilitySkillQ();
        EnHanceCategory_Text->SetText(FText::FromString("R Skill Enhance"));
        break;

        case EEnhanceCategory::Enhance_E:
        SkillNumber = OwnerCharacter->GetESkill();
        OriginSkillAbilityList = OwnerCharacter->GetAbilitySkillE();
        EnHanceCategory_Text->SetText(FText::FromString("E Skill Enhance"));

        break;
        case EEnhanceCategory::Enhance_R:
        EnHanceCategory_Text->SetText(FText::FromString("R Skill Enhance"));
        break;

        case EEnhanceCategory::Enhance_Passive:
        EnHanceCategory_Text->SetText(FText::FromString("Passive Skill Enhance"));
        break;

        case EEnhanceCategory::Enhance_Dash:
        EnHanceCategory_Text->SetText(FText::FromString("Dash Skill Enhance"));
        break;

        case EEnhanceCategory::Enhance_LeftMouse:
        EnHanceCategory_Text->SetText(FText::FromString("LeftMouse Attack Enhance"));
        break;

        case EEnhanceCategory::Enhance_RightMouse:
        EnHanceCategory_Text->SetText(FText::FromString("RightMouse Attack Enhance"));
        break;
    }
    EnhanceListing();
}

void UAPEnhanceChoice::EnhanceListing()
{
    if(!OwnerCharacter) return;

    switch (SkillNumber)
    {
        case ESkillNumber::Skill_1:
        EnableSkillAbilityList = OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber1()->EnableSkillAbilityList;
        break;

        case ESkillNumber::Skill_2:
        EnableSkillAbilityList = OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber2()->EnableSkillAbilityList;
        break;

        case ESkillNumber::Skill_3:
        EnableSkillAbilityList = OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber3()->EnableSkillAbilityList;
        break;

        case ESkillNumber::Skill_4:
        EnableSkillAbilityList = OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber4()->EnableSkillAbilityList;
        break;

        case ESkillNumber::Skill_5:
        EnableSkillAbilityList = OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber5()->EnableSkillAbilityList;
        break;

        case ESkillNumber::Skill_6:
        EnableSkillAbilityList = OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber6()->EnableSkillAbilityList;
        break;

        case ESkillNumber::Skill_7:
        EnableSkillAbilityList = OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber7()->EnableSkillAbilityList;
        break;

        case ESkillNumber::Skill_8:
        EnableSkillAbilityList = OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber8()->EnableSkillAbilityList;
        break;

        case ESkillNumber::Skill_9:
        EnableSkillAbilityList = OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber9()->EnableSkillAbilityList;
        break;

        case ESkillNumber::Skill_10:
        EnableSkillAbilityList = OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber10()->EnableSkillAbilityList;
        break;

        case ESkillNumber::Skill_11:
        EnableSkillAbilityList = OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber11()->EnableSkillAbilityList;
        break;

        case ESkillNumber::Skill_12:
        EnableSkillAbilityList = OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber12()->EnableSkillAbilityList;

        case ESkillNumber::Skill_13:
        EnableSkillAbilityList = OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber13()->EnableSkillAbilityList;
        break;

        case ESkillNumber::Skill_14:
        EnableSkillAbilityList = OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->GetSkillNumber14()->EnableSkillAbilityList;
        break;
    }
    
}

void UAPEnhanceChoice::SetChoiceButton()
{
    ChoiceSlots.Empty();

    for(int32 i = 0; i<3; i++)
    {
        auto ChoiceSlot = CreateWidget<UChoiceButton>(this, ChoiceSlotClass);
        ChoiceWrapBox->AddChildToWrapBox(ChoiceSlot);
        ChoiceSlots.Add(ChoiceSlot);
    }
    for(int32 i = 0; i<ChoiceSlots.Num(); i++)
    {
        float EnhanceAppearPercent = FMath::RandRange(0.0f, 100.0f);
        if(EnhanceAppearPercent >= SkillAppearPercent) // 증강 등장
        {
            EnhanceSuffle();
            ChoiceSlots[i]->SetEnhance(this, SkillNumber, SkillAbility);
        }
        else // 스킬 등장
        {
            if(SkillSuffle())
            {
                ChoiceSlots[i]->SetNewSkill(this, NewSkillNumber);
            }
            else
            {
                EnhanceSuffle();
                ChoiceSlots[i]->SetEnhance(this, SkillNumber, SkillAbility);
            }
        }
    }
}

void UAPEnhanceChoice::EnhanceSuffle()
{  
    if(OriginSkillAbilityList.Num() >= EnableSkillAbilityList.Num()) {SkillAbility = ESkillAbility::Ability_None; return;} // 이미 해당 스킬의 증강을 모두 얻었으면, 나중에 추가?

    if(SkillAbilities.IsEmpty())
    {
        for(int32 i = 0; i<EnableSkillAbilityList.Num(); i++)
        {
            SkillAbilities.Add(i);
        }   

        int32 LastIndex = SkillAbilities.Num() - 1;

        for (int32 i = 0; i <= LastIndex; i += 1) 
        {
            int32 Index = FMath::RandRange(i, LastIndex);
            if (i == Index) 
            {
                continue;
            }
            SkillAbilities.Swap(i, Index);
        }
    }

    if(OriginSkillAbilityList.Contains(EnableSkillAbilityList[SkillAbilities.Top()]))
    {
        SkillAbilities.Pop();
        EnhanceSuffle();
    }
    else
    {
        SkillAbility = EnableSkillAbilityList[SkillAbilities.Top()]; SkillAbilities.Pop();
    }
    
    return;
}

bool UAPEnhanceChoice::SkillSuffle()
{
    const UEnum* SkillNum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.ESkillNumber"), true); if(!SkillNum) return false;
    uint8 NewSkillNum = SkillNum->NumEnums() - 1;

    if(NewSkills.IsEmpty())
    {
        for(int32 i = 1; i<NewSkillNum; i++)
        {
            if(OwnerCharacter->HavingSkill.Contains((ESkillNumber)i)) continue;

            NewSkills.Add(i);
        }
        if(NewSkills.IsEmpty()) return false;

        int32 LastIndex = NewSkills.Num() - 1;
        for (int32 i = 0; i <= LastIndex; i += 1) 
        {
            int32 Index = FMath::RandRange(i, LastIndex);
            if (i == Index) 
            {
                continue;
            }
            NewSkills.Swap(i, Index);
        }
    }

    NewSkillNumber = (ESkillNumber)NewSkills.Top(); NewSkills.Pop();
    return true;
}

void UAPEnhanceChoice::OnReroll()
{
    if(!OwnerCharacter) return;
    
    auto Item = OwnerCharacter->GetInventory()->FindItembyId(TEXT("test_005")); if(!Item) return;
    if(Item->Quantity == 0) return;
    OwnerCharacter->GetInventory()->RemoveAmountOfItem(Item, 1);
    RerollDice_Text->SetText(FText::FromString(FString::FromInt(Item->Quantity)));

    for(int32 i = 0; i<ChoiceSlots.Num(); i++)
    {
        ChoiceSlots[i]->RemoveFromParent();
    }
    SetChoiceButton();

}

void UAPEnhanceChoice::ApplyEnhance(ESkillAbility UpdateSkillAbility)
{
    if(!OwnerCharacter) return;

    OriginSkillAbilityList.Add(UpdateSkillAbility);   

    UE_LOG(LogTemp, Display, TEXT("Your %d"), OriginSkillAbilityList.Num());

    switch (EnhanceCategory)
    {
        case EEnhanceCategory::Enhance_Q:
        OwnerCharacter->SetAbilitySkillQ(OriginSkillAbilityList);
        break;
        case EEnhanceCategory::Enhance_E:
        OwnerCharacter->SetAbilitySkillE(OriginSkillAbilityList);
        break;
        case EEnhanceCategory::Enhance_R:
        OwnerCharacter->SetAbilitySkillR(OriginSkillAbilityList);
        break;
        case EEnhanceCategory::Enhance_Passive:

        break;
        case EEnhanceCategory::Enhance_Dash:

        break;
        case EEnhanceCategory::Enhance_LeftMouse:

        break;
        case EEnhanceCategory::Enhance_RightMouse:

        break;
    }

    RemoveFromParent();
    auto PC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!PC) return;
    PC->SetPause(false);
}

void UAPEnhanceChoice::ApplyNewSkill(ESkillNumber UpdateSkillNumber)
{
    OriginSkillAbilityList.Empty();
    switch (EnhanceCategory)
    {
        case EEnhanceCategory::Enhance_Q:
        OwnerCharacter->SetQSkill(UpdateSkillNumber);
        OwnerCharacter->SetAbilitySkillQ(OriginSkillAbilityList);
        break;
        case EEnhanceCategory::Enhance_E:
        OwnerCharacter->SetESkill(UpdateSkillNumber);
        OwnerCharacter->SetAbilitySkillE(OriginSkillAbilityList);
        break;
        // case ESkillKey::R:
        // OwnerCharacter->SetRSkill(UpdateSkillNumber);
        // OwnerCharacter->SetAbilitySkillR(OriginSkillAbilityList);
        // break;
    }
    OwnerCharacter->SetHavingSkills();
    OwnerCharacter->SetRSkill();

    RemoveFromParent();
    auto PC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!PC) return;
    PC->SetPause(false);
}
