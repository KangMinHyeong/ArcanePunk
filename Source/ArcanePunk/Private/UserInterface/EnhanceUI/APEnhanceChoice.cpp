
#include "UserInterface/EnhanceUI/APEnhanceChoice.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Items/APItemBase.h"
#include "ArcanePunk/Public/Components/APInventoryComponent.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Components/WrapBox.h"
#include "UserInterface/EnhanceUI/ChoiceButton.h"
#include "UserInterface/APHUD.h"

void UAPEnhanceChoice::NativeConstruct()
{
    Super::NativeConstruct();

    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn()); if(!OwnerCharacter.IsValid()) return;

    // InitSuffle();
    SetPauseGame();
}

void UAPEnhanceChoice::SetPauseGame()
{
    if(!OwnerCharacter.IsValid()) return;
    auto PC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!PC) return;

    PC->SetPause(true);
}

void UAPEnhanceChoice::TextSetting()
{
    if(!OwnerCharacter.IsValid()) return;
    
    if(EnHanceType == EEnHanceType::Silver) {EnHanceType_Text->SetText(FText::FromName(TEXT_Silver));}
    else if(EnHanceType == EEnHanceType::Gold) {EnHanceType_Text->SetText(FText::FromName(TEXT_Gold));}
    else if(EnHanceType == EEnHanceType::Platinum) {EnHanceType_Text->SetText(FText::FromName(TEXT_Platinum));}
    
    
    auto Item = OwnerCharacter->GetInventory()->FindItembyId(TEXT("test_005")); 
    if(!Item){ RerollDice_Text->SetText(FText::FromString(FString::FromInt(0)));}
    else { RerollDice_Text->SetText(FText::FromString(FString::FromInt(Item->Quantity)));}
}

void UAPEnhanceChoice::InitType(EEnhanceCategory UpdateEnhanceCategory, EEnHanceType UpdateEnHanceType)
{
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn()); if(!OwnerCharacter.IsValid()) return;
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
    if(!OwnerCharacter.IsValid()) return;

    switch (EnhanceCategory)
    {
        case EEnhanceCategory::Enhance_Q:
        SkillNumber = OwnerCharacter->GetQSkill();
        // OriginSkillAbilityList = OwnerCharacter->GetAbilitySkillQ();
        EnHanceCategory_Text->SetText(FText::FromString("Q Skill Enhance"));
        SkillAbilityNestingData = OwnerCharacter->GetQSkillNumber()->SkillAbilityNestingData;
        break;

        case EEnhanceCategory::Enhance_E:
        SkillNumber = OwnerCharacter->GetESkill();
        // OriginSkillAbilityList = OwnerCharacter->GetAbilitySkillE();
        EnHanceCategory_Text->SetText(FText::FromString("E Skill Enhance"));
        SkillAbilityNestingData = OwnerCharacter->GetESkillNumber()->SkillAbilityNestingData;

        break;
        case EEnhanceCategory::Enhance_R:
        EnHanceCategory_Text->SetText(FText::FromString("R Skill Enhance"));
        // SkillAbilityNestingData = OwnerCharacter->GetRSkillNumber()->SkillAbilityNestingData;
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
    // if(!OwnerCharacter.IsValid()) return;

    // TWeakObjectPtr<USkillNumberBase> SkillNum = OwnerCharacter->GetAPSkillHubComponent()->GetSKillNumberComponent(SkillNumber);
	// if(SkillNum.IsValid())
	// {
	// 	EnableSkillAbilityList = SkillNum->EnableSkillAbilityList;
	// } 
    
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
            // ChoiceSlots[i]->SetEnhance(this, SkillNumber, SkillAbility);
            ChoiceSlots[i]->SetEnhance(this, SkillNumber, SkillAbilities.Top(), PlusAbilityNum);
            SkillAbilities.Pop();
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
                ChoiceSlots[i]->SetEnhance(this, SkillNumber, SkillAbilities.Top(), PlusAbilityNum);
                SkillAbilities.Pop();
            }
        }
    }
}

void UAPEnhanceChoice::EnhanceSuffle()
{  
    // if(OriginSkillAbilityList.Num() >= EnableSkillAbilityList.Num()) {SkillAbility = ESkillAbility::Ability_None; return;} // 이미 해당 스킬의 증강을 모두 얻었으면, 나중에 추가?

    // if(SkillAbilities.IsEmpty())
    // {
    //     for(int32 i = 0; i<EnableSkillAbilityList.Num(); i++)
    //     {
    //         SkillAbilities.Add(i);
    //     }   

    //     int32 LastIndex = SkillAbilities.Num() - 1;

    //     for (int32 i = 0; i <= LastIndex; i += 1) 
    //     {
    //         int32 Index = FMath::RandRange(i, LastIndex);
    //         if (i == Index) 
    //         {
    //             continue;
    //         }
    //         SkillAbilities.Swap(i, Index);
    //     }
    // }

    // if(OriginSkillAbilityList.Contains(EnableSkillAbilityList[SkillAbilities.Top()]))
    // {
    //     SkillAbilities.Pop();
    //     EnhanceSuffle();
    // }
    // else
    // {
    //     SkillAbility = EnableSkillAbilityList[SkillAbilities.Top()]; SkillAbilities.Pop();
    // }
    // const UEnum* SkillNum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.ESkillNumber"), true); if(!SkillNum) return;
    // FName Tets = TEXT("Skill_8"); FName Tets2 = TEXT("Skill_10"); 
    // FName EnumToName = SkillNum->GetNameByValue((int64)SkillNumber);
    // UE_LOG(LogTemp, Display, TEXT("Your %s  %s  %s  %s"), SkillAbilityNestingData.SkillName, EnumToName, Tets, Tets2); return;
    auto DataTable = SkillAbilityDataTable->FindRow<FSkillAbilityDataTable>(SkillAbilityNestingData.SkillName, SkillAbilityNestingData.SkillName.ToString()); if(!DataTable) return;
    TMap<uint8, uint16> AbilityNestingNum;
    
    uint8 Num = 0;
    if(EnHanceType == EEnHanceType::Silver) {Num = DataTable->SilverAbilityInformation.Num(); AbilityNestingNum = SkillAbilityNestingData.SilverAbilityNestingNum;}
    else if(EnHanceType == EEnHanceType::Gold) {Num = DataTable->GoldAbilityInformation.Num(); AbilityNestingNum = SkillAbilityNestingData.GoldAbilityNestingNum;}
    else {Num = DataTable->PlatinumAbilityInformation.Num(); AbilityNestingNum = SkillAbilityNestingData.PlatinumAbilityNestingNum;}

    if(SkillAbilities.IsEmpty())
    {
        for(int32 i = 1; i<=Num; i++)
        {
            SkillAbilities.Add(i);
        }   

        int32 LastIndex = SkillAbilities.Num()-1;

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

    
    if(AbilityNestingNum.Contains(SkillAbilities.Top()))
    {
        bool Nestable = false;
        if(EnHanceType == EEnHanceType::Silver) {Nestable  = DataTable->SilverAbilityInformation[SkillAbilities.Top()].Nestable;}
        else if(EnHanceType == EEnHanceType::Gold) {Nestable  = DataTable->GoldAbilityInformation[SkillAbilities.Top()].Nestable;}
        else {Nestable  = DataTable->PlatinumAbilityInformation[SkillAbilities.Top()].Nestable;}

        if(!Nestable) {SkillAbilities.Pop(); EnhanceSuffle();}
        else {PlusAbilityNum = AbilityNestingNum[SkillAbilities.Top()] + 1; }
    }
    else
    {
	    PlusAbilityNum = 1;
    }
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
    if(!OwnerCharacter.IsValid()) return;
    
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

void UAPEnhanceChoice::ApplyEnhance(uint8 UpdateSkillAbility, uint16 UpdateNestingNumb )
{
    if(!OwnerCharacter.IsValid()) return;

    // OriginSkillAbilityList.Add(UpdateSkillAbility);   

    // switch (EnhanceCategory)
    // {
    //     case EEnhanceCategory::Enhance_Q:
    //     OwnerCharacter->SetAbilitySkillQ(OriginSkillAbilityList);
    //     break;
    //     case EEnhanceCategory::Enhance_E:
    //     OwnerCharacter->SetAbilitySkillE(OriginSkillAbilityList);
    //     break;
    //     case EEnhanceCategory::Enhance_R:
    //     OwnerCharacter->SetAbilitySkillR(OriginSkillAbilityList);
    //     break;
    //     case EEnhanceCategory::Enhance_Passive:

    //     break;
    //     case EEnhanceCategory::Enhance_Dash:

    //     break;
    //     case EEnhanceCategory::Enhance_LeftMouse:

    //     break;
    //     case EEnhanceCategory::Enhance_RightMouse:

    //     break;
    // }

    if(EnHanceType == EEnHanceType::Silver) {OwnerCharacter->GetQSkillNumber()->SkillAbilityNestingData.SilverAbilityNestingNum.Add(UpdateSkillAbility, UpdateNestingNumb);}
    else if(EnHanceType == EEnHanceType::Gold) {OwnerCharacter->GetQSkillNumber()->SkillAbilityNestingData.GoldAbilityNestingNum.Add(UpdateSkillAbility, UpdateNestingNumb);}
    else {OwnerCharacter->GetQSkillNumber()->SkillAbilityNestingData.PlatinumAbilityNestingNum.Add(UpdateSkillAbility, UpdateNestingNumb);}

    RemoveFromParent();
    auto PC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!PC) return;
    PC->SetPause(false);

}

void UAPEnhanceChoice::ApplyNewSkill(ESkillNumber UpdateSkillNumber)
{
    // OriginSkillAbilityList.Empty();
    switch (EnhanceCategory)
    {
        case EEnhanceCategory::Enhance_Q:
        OwnerCharacter->SetQSkill(UpdateSkillNumber);
        // OwnerCharacter->SetAbilitySkillQ(OriginSkillAbilityList);
        OwnerCharacter->GetAPSkillHubComponent()->UpdateSkill_Q();
        OwnerCharacter->GetAPHUD()->OnHightLightSkill.Broadcast(ESkillKey::Q);
        break;
        case EEnhanceCategory::Enhance_E:
        OwnerCharacter->SetESkill(UpdateSkillNumber);
        // OwnerCharacter->SetAbilitySkillE(OriginSkillAbilityList);
        OwnerCharacter->GetAPSkillHubComponent()->UpdateSkill_E();
        OwnerCharacter->GetAPHUD()->OnHightLightSkill.Broadcast(ESkillKey::E);
        break;
    }
    OwnerCharacter->SetHavingSkills();
    OwnerCharacter->SetRSkill();
    OwnerCharacter->GetAPHUD()->OnHightLightSkill.Broadcast(ESkillKey::R);

    RemoveFromParent();
    auto PC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!PC) return;
    PC->SetPause(false);
}
