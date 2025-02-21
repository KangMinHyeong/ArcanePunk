
#include "UserInterface/EnhanceUI/APEnhanceChoice.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Items/APItemBase.h"
#include "ArcanePunk/Public/Components/Character/APInventoryComponent.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Components/HorizontalBox.h"
#include "Components/SizeBox.h"
#include "UserInterface/EnhanceUI/ChoiceButton.h"
#include "UserInterface/HUD/APHUD.h"
#include "GameInstance/APGameInstance.h"
#include "Components/HorizontalBoxSlot.h"

void UAPEnhanceChoice::NativeConstruct()
{
    Super::NativeConstruct();

    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn()); if(!OwnerCharacter.IsValid()) return;

    // InitSuffle();
    SetPauseGame();

    APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI.IsValid()) return;

    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()),Text_CurrentReroll, EStringRowName::CurrentReroll); 
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()),Text_Count, EStringRowName::Count); 
    
    UAPSoundSubsystem::PlayUIOpenSound(UAPGameInstance::GetSoundGI(GetWorld()));
}

void UAPEnhanceChoice::SetPauseGame()
{
    auto PC = Cast<AArcanePunkPlayerController>(GetOwningPlayer()); if(!PC) return;
    PC->SetPause(true);
}

void UAPEnhanceChoice::OnDisplayChoiceButton()
{
    ChoiceSlots[ChoiceNumb]->OnChoice_FadeIn();
    switch (ChoiceNumb)
    {
    case 0:
        ChoiceNumb = 1;
        break;
    case 1:
        ChoiceNumb = 2;
        break;
    
    case 2:
        ChoiceNumb = 0;
        break;
    }
}

void UAPEnhanceChoice::OnDisplayRerollButton()
{
    for(auto & Choice : ChoiceSlots) Choice->OnRerollButton_FadeIn();
}

void UAPEnhanceChoice::OnPlayingChoiceButton(uint8 CurrentNum)
{
    // ChoiceSlots[ChoiceNumb]->OnChoice_FadeIn();
}

void UAPEnhanceChoice::InitType(EEnhanceCategory UpdateEnhanceCategory, EEnHanceType UpdateEnHanceType, uint8 UpdateEnhanceSkillNum)
{
    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn()); if(!OwnerCharacter.IsValid()) return;
    APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI.IsValid()) return;
    DataTableGI = Cast<UAPDataTableSubsystem>(APGI->GetSubsystemBase(UAPDataTableSubsystem::StaticClass())); if(!DataTableGI.IsValid()) return;   
    
    OnBackGround_FadeIn();

    EnhanceCategory = UpdateEnhanceCategory;
    EnHanceType = UpdateEnHanceType;
    EnhanceSkillNum = UpdateEnhanceSkillNum;

    NewSkills.Empty(); SkillAbilities.Empty(); ChoiceSizeBoxes.Empty();

    ChoiceSizeBoxes.Emplace(ChoiceBox_1); 
    ChoiceSizeBoxes.Emplace(ChoiceBox_2);
    ChoiceSizeBoxes.Emplace(ChoiceBox_3);

    InitTypeSetting();
    // SetAbility(); 새로 만들기 위해 빼놈
    SetChoiceButton();
    
    Cancel_Button->OnClicked.AddDynamic(this, &UAPEnhanceChoice::OnClick_Cancel);
    Cancel_Button->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);
}

void UAPEnhanceChoice::InitTypeSetting()
{
    if(!OwnerCharacter.IsValid()) return;

    if(EnHanceType == EEnHanceType::Silver) 
    {
        UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), EnHanceType_Text, EStringRowName::Tier_3);
        ChoiceBorder->SetBrushColor(SilverColor);
    }
    else if(EnHanceType == EEnHanceType::Gold) 
    {
        UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), EnHanceType_Text, EStringRowName::Tier_2);
        ChoiceBorder->SetBrushColor(GoldColor);
    }
    else 
    {
        UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), EnHanceType_Text, EStringRowName::Tier_1);
        ChoiceBorder->SetBrushColor(PlatinumColor);
    }
    
    auto Item = OwnerCharacter->GetInventory()->FindItembyId(TEXT("Dice")); 
    if(!Item){ RerollDice_Text->SetText(FText::FromString(FString::FromInt(0)));}
    else { RerollDice_Text->SetText(FText::FromString(FString::FromInt(Item->Quantity)));}
}

void UAPEnhanceChoice::SetAbility()
{
    if(!OwnerCharacter.IsValid()) return;
    if(!DataTableGI.IsValid()) return; 

    switch (EnhanceCategory)
    {
        case EEnhanceCategory::Enhance_Q:
        SkillNumber = (uint8)OwnerCharacter->GetQSkill();
        SkillAbilityNestingData = OwnerCharacter->GetQSkillNumber()->GetSkillAbilityNestingData();
        break;

        case EEnhanceCategory::Enhance_E:
        SkillNumber = (uint8)OwnerCharacter->GetESkill();
        SkillAbilityNestingData = OwnerCharacter->GetESkillNumber()->GetSkillAbilityNestingData();

        break;
        case EEnhanceCategory::Enhance_R:
        SkillNumber = (uint8)OwnerCharacter->GetRSkill();
        SkillAbilityNestingData = OwnerCharacter->GetRSkillNumber()->GetSkillAbilityNestingData();
        break;

        case EEnhanceCategory::Enhance_Passive:
        PassiveNestingData = OwnerCharacter->GetPassiveSkills();
        break;
    }

    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), EnHanceCategory_Text, EStringRowName::Message_EnhanceChoice);
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
        ChoiceSlot->SetChoiceIndexNum(i);
        ChoiceSlot->InitChoice(this);

        ChoiceSlots.Emplace(ChoiceSlot);       

        ChoiceSizeBoxes[i]->AddChild(ChoiceSlot); 
    }

    // 리뉴얼동안 주석 처리 (초이스 버튼에서 처리리)
    // for(int32 i = 0; i<ChoiceSlots.Num(); i++)
    // {
    //     if(EnhanceCategory == EEnhanceCategory::Enhance_Q || EnhanceCategory == EEnhanceCategory::Enhance_E || EnhanceCategory == EEnhanceCategory::Enhance_R)
    //     {
    //         ActiveSuffle(i); // Active
    //     }
    //     else
    //     {
    //         InitPassiveSuffle(i); // Passive
    //     } 
    // }       
}

void UAPEnhanceChoice::ActiveSuffle(uint8 Index)
{
    float EnhanceAppearPercent = FMath::RandRange(0.0f, 100.0f);
    float CurrentSkillAppearPercent = SkillAppearPercent;
    if(EnhanceCategory == EEnhanceCategory::Enhance_R) CurrentSkillAppearPercent = 0.0f;

    if(EnhanceAppearPercent >= CurrentSkillAppearPercent) // 증강 등장
    {
        EnhanceSuffle(SkillAbilityNestingData.SkillName);
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
            EnhanceSuffle(SkillAbilityNestingData.SkillName);
            ChoiceSlots[Index]->SetEnhance(this, SkillNumber, SkillAbilities.Top(), PlusAbilityNum);
            SkillAbilities.Pop();
        }
    } 
    
}

void UAPEnhanceChoice::InitPassiveSuffle(uint8 Index)
{
    if(PassiveNestingData.Num() == 0) 
    {
        PassiveSuffle(); 
        ChoiceSlots[Index]->SetNewSkill(this, NewPassiveNumber);
        return;
    } // 스킬 셔플
    
    float EnhanceAppearPercent = FMath::RandRange(0.0f, 100.0f);

    if(EnhanceAppearPercent >= PassiveSkillAppearPercent) // 증강 등장
    {
        InitPassiveSkillName();
        EnhanceSuffle(PassiveSkills.Top());
        ChoiceSlots[Index]->SetEnhance(this, SkillNumber, SkillAbilities.Top(), PlusAbilityNum);
        SkillAbilities.Pop();
    }
    else // 스킬 등장
    {
        if(PassiveSuffle())
        {
            ChoiceSlots[Index]->SetNewSkill(this, NewPassiveNumber);
        }
        else // 스킬을 이미 다 한번씩 획득한 경우 -> 증강으로
        {
            InitPassiveSkillName();
            EnhanceSuffle(PassiveSkills.Top());
            ChoiceSlots[Index]->SetEnhance(this, SkillNumber, SkillAbilities.Top(), PlusAbilityNum);
            SkillAbilities.Pop();
        }
    }     
}

void UAPEnhanceChoice::InitPassiveSkillName()
{
    if(!PassiveSkills.IsEmpty()) return;

    const UEnum* SkillNum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.EPassiveNumber"), true); if(!SkillNum) return;
    

    if(EnhanceSkillNum != 0)
    {
        SkillNumber = EnhanceSkillNum;
        FName PassiveName = FName(*SkillNum->GetNameStringByValue(SkillNumber));
        PassiveSkills.Emplace(PassiveName);
        SkillAbilityNestingData = PassiveNestingData[SkillNumber];
        SkillAbilityNestingData.SkillName = PassiveName;
        return;
    }
    else
    {
        TArray<uint8> PassiveSkillKeys;
        for(auto It : PassiveNestingData)
        {
            PassiveSkillKeys.Emplace(It.Key);
        }
        while(PassiveSkills.Num() != PassiveSkillKeys.Num())
        {
            uint8 CurrentNum = FMath::RandRange(0, PassiveSkillKeys.Num() - 1);
            
            FName PassiveName = (FName(*SkillNum->GetNameStringByValue(PassiveSkillKeys[CurrentNum])));
            if(!PassiveSkills.Contains(PassiveName))
            {
                PassiveSkills.Emplace(PassiveName);
                SkillNumber = PassiveSkillKeys[CurrentNum];
                SkillAbilityNestingData = PassiveNestingData[SkillNumber];
                SkillAbilityNestingData.SkillName = PassiveName;
            }
        }
    }
}

void UAPEnhanceChoice::EnhanceSuffle(FName SkillName)
{  
    if(!OwnerCharacter.IsValid()) return; 
    if(!DataTableGI.IsValid()) return;  
    auto DataTable = DataTableGI->GetSkillAbilityRowDataTable()->FindRow<FSkillAbilityRowNameData>(SkillName, SkillName.ToString()); if(!DataTable) return;
    
    TArray<FString> RowName;
    uint8 Num = 0;
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
            SkillAbilities.Emplace(i);
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
        AbilityData = DataTableGI->GetSilverAbilityDataTable()->FindRow<FSkillAbilityDataSheet>(FName(*RowName[SkillAbilities.Top()]), RowName[SkillAbilities.Top()]);
        break;

        case EEnHanceType::Gold:
        AbilityData = DataTableGI->GetGoldAbilityDataTable()->FindRow<FSkillAbilityDataSheet>(FName(*RowName[SkillAbilities.Top()]), RowName[SkillAbilities.Top()]);
        break;

        case EEnHanceType::Platinum:
        AbilityData = DataTableGI->GetPlatinumAbilityDataTable()->FindRow<FSkillAbilityDataSheet>(FName(*RowName[SkillAbilities.Top()]), RowName[SkillAbilities.Top()]);
        break;
    }
    if(!AbilityData) return;
    
    
    if(AbilityNestingNum.Contains(SkillAbilities.Top()+1))
    {
        bool Nestable = AbilityData->Nestable;
        if(!Nestable) {SkillAbilities.Pop(); EnhanceSuffle(SkillName);}
        else 
        {
            PlusAbilityNum = AbilityNestingNum[SkillAbilities.Top()+1];
            if(PlusAbilityNum == AbilityData->MaxNesting) {SkillAbilities.Pop(); EnhanceSuffle(SkillName);}
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
        
            NewSkills.Emplace(i);
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

    NewSkillNumber = NewSkills.Top(); NewSkills.Pop();
    return true;
}

bool UAPEnhanceChoice::PassiveSuffle()
{
    const UEnum* SkillNum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.EPassiveNumber"), true); if(!SkillNum) return false;
    uint8 NewSkillNum = SkillNum->NumEnums() - 2;

    if(NewPassives.IsEmpty())
    {
        if(OwnerCharacter->GetPassiveSkills().Num() == NewSkillNum) return false;

        for(int32 i = 1; i<=NewSkillNum; i++)
        {
            if(OwnerCharacter->GetPassiveSkills().Contains(i)) continue;
        
            NewPassives.Emplace(i);
        }
        if(NewPassives.IsEmpty()) return false;

        int32 LastIndex = NewPassives.Num() - 1;
        for (int32 i = 0; i <= LastIndex; i += 1) 
        {
            int32 Index = FMath::RandRange(i, LastIndex);
            if (i == Index) 
            {
                continue;
            }
            NewPassives.Swap(i, Index);
        }
    }

    NewPassiveNumber = NewPassives.Top(); NewPassives.Pop();
    return true;
}

void UAPEnhanceChoice::OnReroll(uint8 ChoiceIndexNum)
{
    if(!OwnerCharacter.IsValid()) return;
    
    auto Item = OwnerCharacter->GetInventory()->FindItembyId(TEXT("Dice")); if(!Item) return;
    if(Item->Quantity == 0) return;
    OwnerCharacter->GetInventory()->RemoveAmountOfItem(Item, 1);
    RerollDice_Text->SetText(FText::FromString(FString::FromInt(Item->Quantity)));

    ChoiceSlots[ChoiceIndexNum]->RemoveFromParent();
    ChoiceSizeBoxes[ChoiceIndexNum]->ClearChildren();
   
    auto ChoiceSlot = CreateWidget<UChoiceButton>(this, ChoiceSlotClass);
    ChoiceSlot->SetChoiceIndexNum(ChoiceIndexNum);
    ChoiceSlots[ChoiceIndexNum] = ChoiceSlot;
    ChoiceSlots[ChoiceIndexNum]->OnChoice_FadeIn();
    ChoiceSlots[ChoiceIndexNum]->OnRerollButton_FadeIn();

    ChoiceSizeBoxes[ChoiceIndexNum]->AddChild(ChoiceSlot); 

    if(EnhanceCategory == EEnhanceCategory::Enhance_Q || EnhanceCategory == EEnhanceCategory::Enhance_E || EnhanceCategory == EEnhanceCategory::Enhance_R)
    {
        ActiveSuffle(ChoiceIndexNum); // Active
    }
    else
    {
        InitPassiveSuffle(ChoiceIndexNum); // Passive
    } 
}

void UAPEnhanceChoice::OnClick_Cancel()
{
    UAPSoundSubsystem::PlayUICloseSound(UAPGameInstance::GetSoundGI(GetWorld()));

    RemoveFromParent();
    auto PC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!PC) return;
    PC->SetPause(false);
}

void UAPEnhanceChoice::ApplyEnhance(uint8 UpdateSkillNumber, uint8 UpdateSkillAbility, uint16 UpdateNestingNumb, uint16 MaxNestingNumb)
{
    if(!OwnerCharacter.IsValid()) return;

    UpdateSkillAbility++;  
    switch (EnhanceCategory)
    {
    case EEnhanceCategory::Enhance_Q:

        if(EnHanceType != EEnHanceType::Platinum) {OwnerCharacter->GetQSkillNumber()->ClearSkillAbilityNestingData(EnHanceType);}
        
        OwnerCharacter->GetQSkillNumber()->AddSkillAbilityNestingData(EnHanceType, MakeTuple(UpdateSkillAbility, UpdateNestingNumb));
        APGI->OnSkillEnhanceDataUpdate.Broadcast(ESkillKey::Q, OwnerCharacter->GetQSkillNumber()->GetSkillAbilityNestingData());
        OwnerCharacter->GetQSkillNumber()->UpdateSkillData();
        break;

    case EEnhanceCategory::Enhance_E:

        if(EnHanceType != EEnHanceType::Platinum) {OwnerCharacter->GetESkillNumber()->ClearSkillAbilityNestingData(EnHanceType);}
        
        OwnerCharacter->GetESkillNumber()->AddSkillAbilityNestingData(EnHanceType, MakeTuple(UpdateSkillAbility, UpdateNestingNumb));
        APGI->OnSkillEnhanceDataUpdate.Broadcast(ESkillKey::E, OwnerCharacter->GetESkillNumber()->GetSkillAbilityNestingData());
        OwnerCharacter->GetESkillNumber()->UpdateSkillData();
        break;

    case EEnhanceCategory::Enhance_R:

        if(EnHanceType != EEnHanceType::Platinum) {OwnerCharacter->GetRSkillNumber()->ClearSkillAbilityNestingData(EnHanceType);}
        
        OwnerCharacter->GetRSkillNumber()->AddSkillAbilityNestingData(EnHanceType, MakeTuple(UpdateSkillAbility, UpdateNestingNumb));
        APGI->OnSkillEnhanceDataUpdate.Broadcast(ESkillKey::R, OwnerCharacter->GetRSkillNumber()->GetSkillAbilityNestingData());
        OwnerCharacter->GetRSkillNumber()->UpdateSkillData();
        break;

    case EEnhanceCategory::Enhance_Passive:
        OwnerCharacter->AddPassive_Enhance( UpdateSkillNumber, EnHanceType, UpdateSkillAbility, UpdateNestingNumb);
        break;
    }
    OwnerCharacter->GetAPHUD()->DisplayEnhanceGauge(UpdateNestingNumb, MaxNestingNumb);

    RemoveFromParent();
    auto PC = Cast<AArcanePunkPlayerController>(GetOwningPlayer()); if(!PC) return;
    PC->SetPause(false);

}

void UAPEnhanceChoice::ApplyNewSkill(uint8 UpdateSkillNumber)
{
    // OriginSkillAbilityList.Empty();
    switch (EnhanceCategory)
    {
    case EEnhanceCategory::Enhance_Q:
        OwnerCharacter->SetQSkill((ESkillNumber)UpdateSkillNumber);
        // OwnerCharacter->SetAbilitySkillQ(OriginSkillAbilityList);
        OwnerCharacter->GetAPSkillHubComponent()->UpdatingSkill_Q();
        OwnerCharacter->GetAPHUD()->GetStatusWidget()->HightLightSkillSlot(ESkillKey::Q);
        APGI->OnSkillEnhanceDataClear.Broadcast(ESkillKey::Q);
        break;

    case EEnhanceCategory::Enhance_E:
        OwnerCharacter->SetESkill((ESkillNumber)UpdateSkillNumber);
        // OwnerCharacter->SetAbilitySkillE(OriginSkillAbilityList);
        OwnerCharacter->GetAPSkillHubComponent()->UpdatingSkill_E();
        OwnerCharacter->GetAPHUD()->GetStatusWidget()->HightLightSkillSlot(ESkillKey::E);
        APGI->OnSkillEnhanceDataClear.Broadcast(ESkillKey::E);
        break;

    case EEnhanceCategory::Enhance_Passive:
        OwnerCharacter->AddPassive((EPassiveNumber)UpdateSkillNumber);
        break;
    }
    OwnerCharacter->SetHavingSkills();
    // OwnerCharacter->SetRSkill();
    // OwnerCharacter->GetAPHUD()->OnHightLightSkill.Broadcast(ESkillKey::R);
    // R 증강은 초기화? GI->OnSkillEnhanceDataUpdate.Broadcast(ESkillKey::R);

    RemoveFromParent();

    uint8 CurrentSkillNum = 0;
    if(EnHanceType == EEnHanceType::Silver)
    {
        auto PC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!PC) return;
        PC->SetPause(false);
    }
    else if(EnHanceType == EEnHanceType::Gold)
    {
        if(EnhanceCategory == EEnhanceCategory::Enhance_Passive) CurrentSkillNum = UpdateSkillNumber;
        OwnerCharacter->GetAPHUD()->DisplayEnhanceChoice(EnhanceCategory, EEnHanceType::Silver, true, CurrentSkillNum);
    }
    else
    {
        if(EnhanceCategory == EEnhanceCategory::Enhance_Passive) CurrentSkillNum = UpdateSkillNumber;
        OwnerCharacter->GetAPHUD()->DisplayEnhanceChoice(EnhanceCategory, EEnHanceType::Gold, true, CurrentSkillNum);
    }
    
}
