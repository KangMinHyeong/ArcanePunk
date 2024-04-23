
#include "UserInterface/EnhanceUI/APEnhanceChoice.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Items/APItemBase.h"
#include "ArcanePunk/Public/Components/APInventoryComponent.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Components/HorizontalBox.h"
#include "Components/SizeBox.h"
#include "UserInterface/EnhanceUI/ChoiceButton.h"
#include "UserInterface/APHUD.h"
#include "GameInstance/APGameInstance.h"
#include "Components/HorizontalBoxSlot.h"

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
    
    Cancel_Button->OnClicked.AddDynamic(this, &UAPEnhanceChoice::OnCancel);
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
        auto SizeBox = NewObject<USizeBox>(this, USizeBox::StaticClass());
        ChoiceSizeBoxes.Add(SizeBox);
        
        auto ChoiceSlot = CreateWidget<UChoiceButton>(this, ChoiceSlotClass);
        ChoiceSlot->SetChoiceIndexNum(i);
        ChoiceSlots.Add(ChoiceSlot);

        auto PanelSlot = ChoiceHorizontalBox->AddChildToHorizontalBox(SizeBox);
        FSlateChildSize SlotSize = PanelSlot->GetSize(); SlotSize.SizeRule = ESlateSizeRule::Fill;
        PanelSlot->SetSize(SlotSize);
        SizeBox->AddChild(ChoiceSlot); 
    }

    if(EnhanceCategory == EEnhanceCategory::Enhance_Q || EnhanceCategory == EEnhanceCategory::Enhance_E || EnhanceCategory == EEnhanceCategory::Enhance_R)
    {
        for(int32 i = 0; i<ChoiceSlots.Num(); i++)
        {
            ActiveSuffle(i); // Active
        }
    }
    else
    {
        InitPassiveSuffle(); // Passive
    }    
}

void UAPEnhanceChoice::ActiveSuffle(uint8 Index)
{
    float EnhanceAppearPercent = FMath::RandRange(0.0f, 100.0f);
    if(EnhanceAppearPercent >= SkillAppearPercent) // 증강 등장
    {
        EnhanceSuffle();
        // ChoiceSlots[i]->SetEnhance(this, SkillNumber, SkillAbility);
        ChoiceSlots[Index]->SetEnhance(this, SkillNumber, SkillAbilities.Top(), PlusAbilityNum);
        SkillAbilities.Pop();
    }
    else // 스킬 등장
    {
        if(SkillSuffle())
        {
            ChoiceSlots[Index]->SetNewSkill(this, NewSkillNumber);
        }
        else // 스킬을 이미 다 한번씩 획득한 경우 -> 증강으로
        {
            EnhanceSuffle();
            ChoiceSlots[Index]->SetEnhance(this, SkillNumber, SkillAbilities.Top(), PlusAbilityNum);
            SkillAbilities.Pop();
        }
    } 
    
}

void UAPEnhanceChoice::InitPassiveSuffle()
{

}

void UAPEnhanceChoice::EnhanceSuffle()
{  
    if(!OwnerCharacter.IsValid()) return;
    auto DataTable = SkillAbilityRowNameData->FindRow<FSkillAbilityRowNameData>(SkillAbilityNestingData.SkillName, SkillAbilityNestingData.SkillName.ToString()); if(!DataTable) return;
    
    TArray<FString> RowName;
    uint8 Num = 0;
    // if(EnHanceType == EEnHanceType::Silver) {Num = DataTable->SilverAbilityInformation.Num(); AbilityNestingNum = SkillAbilityNestingData.SilverAbilityNestingNum;}
    // else if(EnHanceType == EEnHanceType::Gold) {Num = DataTable->GoldAbilityInformation.Num(); AbilityNestingNum = SkillAbilityNestingData.GoldAbilityNestingNum;}
    // else {Num = DataTable->PlatinumAbilityInformation.Num(); AbilityNestingNum = SkillAbilityNestingData.PlatinumAbilityNestingNum;}
    TMap<uint8, uint16> AbilityNestingNum;
    switch (EnHanceType)
    {
        case EEnHanceType::Silver:
        RowName = DataTable->SilverRowName;
        Num = RowName.Num(); 
        AbilityNestingNum = SkillAbilityNestingData.SilverAbilityNestingNum;
        break;

        case EEnHanceType::Gold:
        RowName = DataTable->GoldRowName;
        Num = RowName.Num(); 
        AbilityNestingNum = SkillAbilityNestingData.GoldAbilityNestingNum;
        break;

        case EEnHanceType::Platinum:
        RowName = DataTable->PlatinumRowName;
        Num = RowName.Num(); 
        AbilityNestingNum = SkillAbilityNestingData.PlatinumAbilityNestingNum;
        break;
    }

    if(SkillAbilities.IsEmpty())
    {
        for(int32 i = 0; i<Num; i++)
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
    
    FSkillAbilityDataSheet* AbilityData = nullptr;
    switch (EnHanceType)
    {
        case EEnHanceType::Silver:
        AbilityData = OwnerCharacter->GetSilverAbilityDataTable()->FindRow<FSkillAbilityDataSheet>(FName(*RowName[SkillAbilities.Top()]), RowName[SkillAbilities.Top()]);
        break;

        case EEnHanceType::Gold:
        AbilityData = OwnerCharacter->GetGoldAbilityDataTable()->FindRow<FSkillAbilityDataSheet>(FName(*RowName[SkillAbilities.Top()]), RowName[SkillAbilities.Top()]);
        break;

        case EEnHanceType::Platinum:
        AbilityData = OwnerCharacter->GetPlatinumAbilityDataTable()->FindRow<FSkillAbilityDataSheet>(FName(*RowName[SkillAbilities.Top()]), RowName[SkillAbilities.Top()]);
        break;
    }
    if(!AbilityData) return;
    
    
    if(AbilityNestingNum.Contains(SkillAbilities.Top()+1))
    {
        bool Nestable = AbilityData->Nestable;
        if(!Nestable) {SkillAbilities.Pop(); EnhanceSuffle();}
        else 
        {
            PlusAbilityNum = AbilityNestingNum[SkillAbilities.Top()+1];
            if(PlusAbilityNum == AbilityData->MaxNesting) {SkillAbilities.Pop(); EnhanceSuffle();}
            else {PlusAbilityNum++;}
        }
    }
    else
    {
	    PlusAbilityNum = 1;
    }
}

bool UAPEnhanceChoice::SkillSuffle()
{
    const UEnum* SkillNum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.ESkillNumber"), true); if(!SkillNum) return false;
    const UEnum* QSkill = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.EQSkill"), true); if(!QSkill) return false;
    const UEnum* ESkill = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.EESkill"), true); if(!ESkill) return false;
    uint8 NewSkillNum = SkillNum->NumEnums() - 1;

    if(NewSkills.IsEmpty())
    {
        for(int32 i = 1; i<NewSkillNum; i++)
        {
            if(OwnerCharacter->HavingSkill.Contains((ESkillNumber)i)) continue;

            if(EnhanceCategory == EEnhanceCategory::Enhance_Q)
            {
                if(!QSkill->IsValidEnumValue(i)) continue;
            }
            else if(EnhanceCategory == EEnhanceCategory::Enhance_E)
            {
                if(!ESkill->IsValidEnumValue(i)) continue;
            }
        
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

void UAPEnhanceChoice::OnReroll(uint8 ChoiceIndexNum)
{
    if(!OwnerCharacter.IsValid()) return;
    
    auto Item = OwnerCharacter->GetInventory()->FindItembyId(TEXT("test_005")); if(!Item) return;
    if(Item->Quantity == 0) return;
    OwnerCharacter->GetInventory()->RemoveAmountOfItem(Item, 1);
    RerollDice_Text->SetText(FText::FromString(FString::FromInt(Item->Quantity)));

    ChoiceSlots[ChoiceIndexNum]->RemoveFromParent();
    ChoiceSizeBoxes[ChoiceIndexNum]->ClearChildren();
   
    auto ChoiceSlot = CreateWidget<UChoiceButton>(this, ChoiceSlotClass);
    ChoiceSlot->SetChoiceIndexNum(ChoiceIndexNum);
    ChoiceSlots[ChoiceIndexNum] = ChoiceSlot;

    ChoiceSizeBoxes[ChoiceIndexNum]->AddChild(ChoiceSlot); 

    ActiveSuffle(ChoiceIndexNum);
}

void UAPEnhanceChoice::OnCancel()
{
    RemoveFromParent();
    auto PC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!PC) return;
    PC->SetPause(false);
}

void UAPEnhanceChoice::ApplyEnhance(uint8 UpdateSkillAbility, uint16 UpdateNestingNumb, uint16 MaxNestingNumb)
{
    if(!OwnerCharacter.IsValid()) return;

    UpdateSkillAbility++;
    auto GI = Cast<UAPGameInstance>(GetGameInstance()); if(!GI) return;
    switch (EnhanceCategory)
    {
    case EEnhanceCategory::Enhance_Q:

        if(EnHanceType == EEnHanceType::Silver) {OwnerCharacter->GetQSkillNumber()->SkillAbilityNestingData.SilverAbilityNestingNum.Empty(); OwnerCharacter->GetQSkillNumber()->SkillAbilityNestingData.SilverAbilityNestingNum.Add(UpdateSkillAbility, UpdateNestingNumb);}
        else if(EnHanceType == EEnHanceType::Gold) {OwnerCharacter->GetQSkillNumber()->SkillAbilityNestingData.GoldAbilityNestingNum.Empty(); OwnerCharacter->GetQSkillNumber()->SkillAbilityNestingData.GoldAbilityNestingNum.Add(UpdateSkillAbility, UpdateNestingNumb);}
        else { OwnerCharacter->GetQSkillNumber()->SkillAbilityNestingData.PlatinumAbilityNestingNum.Add(UpdateSkillAbility, UpdateNestingNumb);}
        
        GI->OnSkillEnhanceDataUpdate.Broadcast(ESkillKey::Q, OwnerCharacter->GetQSkillNumber()->SkillAbilityNestingData);
        OwnerCharacter->GetQSkillNumber()->UpdateSkillData();
        break;

    case EEnhanceCategory::Enhance_E:

        if(EnHanceType == EEnHanceType::Silver) {OwnerCharacter->GetESkillNumber()->SkillAbilityNestingData.SilverAbilityNestingNum.Empty(); OwnerCharacter->GetESkillNumber()->SkillAbilityNestingData.SilverAbilityNestingNum.Add(UpdateSkillAbility, UpdateNestingNumb);}
        else if(EnHanceType == EEnHanceType::Gold) {OwnerCharacter->GetESkillNumber()->SkillAbilityNestingData.GoldAbilityNestingNum.Empty(); OwnerCharacter->GetESkillNumber()->SkillAbilityNestingData.GoldAbilityNestingNum.Add(UpdateSkillAbility, UpdateNestingNumb);}
        else { OwnerCharacter->GetESkillNumber()->SkillAbilityNestingData.PlatinumAbilityNestingNum.Add(UpdateSkillAbility, UpdateNestingNumb);}

        GI->OnSkillEnhanceDataUpdate.Broadcast(ESkillKey::E, OwnerCharacter->GetQSkillNumber()->SkillAbilityNestingData);
        OwnerCharacter->GetESkillNumber()->UpdateSkillData();
        break;
    }
    OwnerCharacter->GetAPHUD()->DisplayEnhanceGauge(UpdateNestingNumb, MaxNestingNumb);

    RemoveFromParent();
    auto PC = Cast<AArcanePunkPlayerController>(GetOwningPlayer()); if(!PC) return;
    PC->SetPause(false);

}

void UAPEnhanceChoice::ApplyNewSkill(ESkillNumber UpdateSkillNumber)
{
    // OriginSkillAbilityList.Empty();
    auto GI = Cast<UAPGameInstance>(GetGameInstance()); if(!GI) return;
    switch (EnhanceCategory)
    {
        case EEnhanceCategory::Enhance_Q:
        OwnerCharacter->SetQSkill(UpdateSkillNumber);
        // OwnerCharacter->SetAbilitySkillQ(OriginSkillAbilityList);
        OwnerCharacter->GetAPSkillHubComponent()->UpdateSkill_Q();
        OwnerCharacter->GetAPHUD()->OnHightLightSkill.Broadcast(ESkillKey::Q);
        GI->OnSkillEnhanceDataClear.Broadcast(ESkillKey::Q);
        break;
        case EEnhanceCategory::Enhance_E:
        OwnerCharacter->SetESkill(UpdateSkillNumber);
        // OwnerCharacter->SetAbilitySkillE(OriginSkillAbilityList);
        OwnerCharacter->GetAPSkillHubComponent()->UpdateSkill_E();
        OwnerCharacter->GetAPHUD()->OnHightLightSkill.Broadcast(ESkillKey::E);
        GI->OnSkillEnhanceDataClear.Broadcast(ESkillKey::E);
        break;
    }
    OwnerCharacter->SetHavingSkills();
    OwnerCharacter->SetRSkill();
    OwnerCharacter->GetAPHUD()->OnHightLightSkill.Broadcast(ESkillKey::R);
    // R 증강은 초기화? GI->OnSkillEnhanceDataUpdate.Broadcast(ESkillKey::R);

    RemoveFromParent();

    if(EnHanceType == EEnHanceType::Silver)
    {
        auto PC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!PC) return;
        PC->SetPause(false);
    }
    else if(EnHanceType == EEnHanceType::Gold)
    {
        OwnerCharacter->GetAPHUD()->DisplayEnhanceChoice(EnhanceCategory, EEnHanceType::Silver, true);
    }
    else
    {
        OwnerCharacter->GetAPHUD()->DisplayEnhanceChoice(EnhanceCategory, EEnHanceType::Gold, true);
    }
}
