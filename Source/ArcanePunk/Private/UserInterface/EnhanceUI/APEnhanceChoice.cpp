
#include "UserInterface/EnhanceUI/APEnhanceChoice.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Items/APItemBase.h"
#include "ArcanePunk/Public/Components/APInventoryComponent.h"

void UAPEnhanceChoice::NativeConstruct()
{
    Super::NativeConstruct();

    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn()); if(!OwnerCharacter) return;

    InitSuffle();
    SetPauseGame();
}

void UAPEnhanceChoice::InitSuffle()
{
    SkillNumber.Empty();

    for(int32 i = 1; i<=30; i++) // 30-> 후에 스킬 개수로 변경
    {
        SkillNumber.Add(i);
    }
    
    int32 LastIndex = SkillNumber.Num() - 1;

    for (int32 i = 0; i <= LastIndex; i += 1) 
    {
        int32 Index = FMath::RandRange(i, LastIndex);
        if (i == Index) 
        {
            continue;
        }
        SkillNumber.Swap(i, Index);
    }
    TextSetting();
}

void UAPEnhanceChoice::SetPauseGame()
{
    if(!OwnerCharacter) return;
    auto PC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!PC) return;

    PC->EnhanceChoiceMode(true);
}

void UAPEnhanceChoice::InitType(ESkillTypeState UpdateSkillTypeState, EEnHanceType UpdateEnHanceType)
{
    SkillTypeState = UpdateSkillTypeState;
    EnHanceType = UpdateEnHanceType;

    BindButton();
}

void UAPEnhanceChoice::TextSetting()
{
    if(!OwnerCharacter) return;
    
    if(EnHanceType == EEnHanceType::Silver) {EnHanceType_Text->SetText(FText::FromName(TEXT_Silver));}
    else if(EnHanceType == EEnHanceType::Gold) {EnHanceType_Text->SetText(FText::FromName(TEXT_Gold));}
    else if(EnHanceType == EEnHanceType::Platinum) {EnHanceType_Text->SetText(FText::FromName(TEXT_Platinum));}
    
    
    if(SkillTypeState == ESkillTypeState::Type_Q) {SkillType_Text1->SetText(FText::FromName(TEXT_EnhanceQ)); SkillType_Text2->SetText(FText::FromName(TEXT_EnhanceQ)); SkillType_Text3->SetText(FText::FromName(TEXT_EnhanceQ));}
    else if(SkillTypeState == ESkillTypeState::Type_E) {SkillType_Text1->SetText(FText::FromName(TEXT_EnhanceE)); SkillType_Text2->SetText(FText::FromName(TEXT_EnhanceE)); SkillType_Text3->SetText(FText::FromName(TEXT_EnhanceE));}
    else if(SkillTypeState == ESkillTypeState::Type_R) {SkillType_Text1->SetText(FText::FromName(TEXT_EnhanceR)); SkillType_Text2->SetText(FText::FromName(TEXT_EnhanceR)); SkillType_Text3->SetText(FText::FromName(TEXT_EnhanceR));}

    Choice1 = SkillNumber.Top(); Choice1_Text->SetText(FText::FromString(FString::FromInt(Choice1))); SkillNumber.Pop();
    Choice2 = SkillNumber.Top(); Choice2_Text->SetText(FText::FromString(FString::FromInt(Choice2))); SkillNumber.Pop();
    Choice3 = SkillNumber.Top(); Choice3_Text->SetText(FText::FromString(FString::FromInt(Choice3))); SkillNumber.Pop();

    auto Item = OwnerCharacter->GetInventory()->FindItembyId(TEXT("test_005")); 
    if(!Item){ RerollDice_Text->SetText(FText::FromString(FString::FromInt(0)));}
    else { RerollDice_Text->SetText(FText::FromString(FString::FromInt(Item->Quantity)));}
    
}

void UAPEnhanceChoice::BindButton()
{   
    Choice1_Button->OnClicked.AddDynamic(this, &UAPEnhanceChoice::OnEnhanveChoice1);
    Choice2_Button->OnClicked.AddDynamic(this, &UAPEnhanceChoice::OnEnhanveChoice2);
    Choice3_Button->OnClicked.AddDynamic(this, &UAPEnhanceChoice::OnEnhanveChoice3);
    Reroll_Button->OnClicked.AddDynamic(this, &UAPEnhanceChoice::OnReroll);
}

void UAPEnhanceChoice::OnEnhanveChoice1()
{
    if(!OwnerCharacter) return;

    // OwnerCharacter->SetEnhanceNumber(Choice1);
    RemoveFromParent();
    auto PC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!PC) return;
    PC->EnhanceChoiceMode(false);
}
void UAPEnhanceChoice::OnEnhanveChoice2()
{
    if(!OwnerCharacter) return;

    // OwnerCharacter->SetEnhanceNumber(Choice2);
    RemoveFromParent();
    auto PC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!PC) return;
    PC->EnhanceChoiceMode(false);
}
void UAPEnhanceChoice::OnEnhanveChoice3()
{
    if(!OwnerCharacter) return;

    // OwnerCharacter->SetEnhanceNumber(Choice3);
    RemoveFromParent();
    auto PC = Cast<AArcanePunkPlayerController>(OwnerCharacter->GetController()); if(!PC) return;
    PC->EnhanceChoiceMode(false);
}

void UAPEnhanceChoice::OnReroll()
{
    if(!OwnerCharacter) return;

    // auto PD = OwnerCharacter->GetPlayerStatus();
    // if(PD.PlayerGoodsData.RerollDice == 0) return;
    // PD.PlayerGoodsData.RerollDice--; OwnerCharacter->SetPlayerStatus(PD);
    
    auto Item = OwnerCharacter->GetInventory()->FindItembyId(TEXT("test_005")); if(!Item) return;
    if(Item->Quantity == 0) return;
    OwnerCharacter->GetInventory()->RemoveAmountOfItem(Item, 1);
    RerollDice_Text->SetText(FText::FromString(FString::FromInt(Item->Quantity)));

    if(SkillNumber.Num() > 3)
    {
        Choice1 = SkillNumber.Top(); Choice1_Text->SetText(FText::FromString(FString::FromInt(Choice1))); SkillNumber.Pop();
        Choice2 = SkillNumber.Top(); Choice2_Text->SetText(FText::FromString(FString::FromInt(Choice2))); SkillNumber.Pop();
        Choice3 = SkillNumber.Top(); Choice3_Text->SetText(FText::FromString(FString::FromInt(Choice3))); SkillNumber.Pop();
    }
    else
    {
        InitSuffle();
    }
}
