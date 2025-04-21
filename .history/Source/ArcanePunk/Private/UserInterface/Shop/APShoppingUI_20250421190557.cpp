
#include "UserInterface/Shop/APShoppingUI.h"

#include "PlayerController/ArcanePunkPlayerController.h"
#include "UserInterface/Shop/ShoppingChoiceUI.h"
#include "UserInterface/Shop/ShoppingListUI.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"
#include "Components/Character/APSkillHubComponent.h"
#include "GameInstance/APGameInstance.h"
#include "UserInterface/HUD/APHUD.h"
#include "Interaction/APInteraction_Shop.h"
#include "Components/Character/APPassiveComponent.h"
#include "Components/TextBlock.h"

void UAPShoppingUI::NativeConstruct()
{
    Super::NativeConstruct();

    OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn());

    Button_Cancel->OnClicked.AddDynamic(this, &UAPShoppingUI::OnClick_Exit);
    Button_Cancel->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);

    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_Information, EStringRowName::Information);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), TextBlock_NoEnough, EStringRowName::NoEnoughGold);

    UAPSoundSubsystem::PlayUIOpenSound(UAPGameInstance::GetSoundGI(GetWorld()));
}

void UAPShoppingUI::InitShopData(AActor* ShopActor, const FShopListData & UpdateShopListData)
{
    ShopListData = UpdateShopListData;
    OwnerShop = Cast<AAPInteraction_Shop>(ShopActor);

    ShoppingList.Empty();
    ScrollBox_List->ClearChildren();
    SizeBox_Choice->ClearChildren();
    InitShoppingList();
}

void UAPShoppingUI::InitShoppingList()
{
    auto Enhance_List = ShopListData.ShopGoodsList_Enhance;
    int32 Number = 0;
    for(auto EnhanceData : Enhance_List)
    {
        auto Enhance_Goods = CreateWidget<UShoppingListUI>(this, ShoppingListUIClass);
        if(Enhance_Goods)
        {
            Enhance_Goods->InitParent(this, Number);
            Enhance_Goods->InitEnhanceData(EnhanceData);
            ShoppingList.Add(Enhance_Goods);
            ScrollBox_List->AddChild(Enhance_Goods);
        }
        Number++;
    }

    auto NewSkill_List = ShopListData.ShopGoodsList_NewSkill;
    for(auto NewSkillData : NewSkill_List)
    {
        auto NewSkil_Goods = CreateWidget<UShoppingListUI>(this, ShoppingListUIClass);
        if(NewSkil_Goods)
        {
            NewSkil_Goods->InitParent(this, Number);
            NewSkil_Goods->InitNewSkillData(NewSkillData);
            ShoppingList.Add(NewSkil_Goods);
            ScrollBox_List->AddChild(NewSkil_Goods);
        }
        Number++;
    }
}

void UAPShoppingUI::OnClick_Exit()
{
    UAPSoundSubsystem::PlayUICloseSound(UAPGameInstance::GetSoundGI(GetWorld()));

    if(OwnerShop.IsValid()) OwnerShop->SetShopData(ShopListData);
    auto PC = Cast<AArcanePunkPlayerController>(GetOwningPlayer());
    if(PC) PC->CloseShoppingUI();
}

void UAPShoppingUI::ClickEnhanceList(const FShopGoodsData_Enhance & EnhanceData, int32 ListNumber)
{
    SizeBox_Choice->ClearChildren();
    if(ShoppingChoice.IsValid()) 
    {
        ShoppingChoice->RemoveFromParent();
    }

    ShoppingChoice = CreateWidget<UShoppingChoiceUI>(this, ShoppingChoiceUIClass);
    if(ShoppingChoice.IsValid())
    {
        ShoppingChoice->InitParent(this, ListNumber);
        ShoppingChoice->InitInformation_Enhance(EnhanceData);
        if(ShoppingChoice.IsValid()) SizeBox_Choice->AddChild(ShoppingChoice.Get());
    }
    
}

void UAPShoppingUI::ClickNewSkillList(const FShopGoodsData_NewSkill & NewSkillData, int32 ListNumber)
{
    SizeBox_Choice->ClearChildren();
    if(ShoppingChoice.IsValid()) 
    {
        ShoppingChoice->RemoveFromParent();
    }

    ShoppingChoice = CreateWidget<UShoppingChoiceUI>(this, ShoppingChoiceUIClass);

    if(ShoppingChoice.IsValid())
    {
        ShoppingChoice->InitParent(this, ListNumber);
        ShoppingChoice->InitInformation_NewSkill(NewSkillData);
        if(ShoppingChoice.IsValid()) SizeBox_Choice->AddChild(ShoppingChoice.Get());
    }

}

void UAPShoppingUI::PurchaseEnhance(int32 ChoiceNumber, FShopGoodsData_Enhance& EnhanceData, TPair<uint8, uint16> AbilityNestingNum)
{
    if(!OwnerCharacter.IsValid()) return;
    auto GI = Cast<UAPGameInstance>(GetGameInstance()); if(!GI) return;

    switch (EnhanceData.GoodsCategory)
    {
    case EEnhanceCategory::Enhance_Q:

        // if(EnhanceData.GoodsType != EEnHanceType::Platinum) {OwnerCharacter->GetQSkillNumber()->ClearSkillAbilityNestingData(EnhanceData.GoodsType);}
        
        // OwnerCharacter->GetQSkillNumber()->AddSkillAbilityNestingData(EnhanceData.GoodsType, AbilityNestingNum);
        // GI->OnSkillEnhanceDataUpdate.Broadcast(ESkillKey::Q, OwnerCharacter->GetQSkillNumber()->GetSkillAbilityNestingData());
        // OwnerCharacter->GetQSkillNumber()->UpdateSkillData();
        break;

    case EEnhanceCategory::Enhance_E:

        // if(EnhanceData.GoodsType != EEnHanceType::Platinum) {OwnerCharacter->GetESkillNumber()->ClearSkillAbilityNestingData(EnhanceData.GoodsType);}
        
        // OwnerCharacter->GetESkillNumber()->AddSkillAbilityNestingData(EnhanceData.GoodsType, AbilityNestingNum);
        // GI->OnSkillEnhanceDataUpdate.Broadcast(ESkillKey::E, OwnerCharacter->GetESkillNumber()->GetSkillAbilityNestingData());
        // OwnerCharacter->GetESkillNumber()->UpdateSkillData();
        break;

    case EEnhanceCategory::Enhance_R:

        // if(EnhanceData.GoodsType != EEnHanceType::Platinum) {OwnerCharacter->GetRSkillNumber()->ClearSkillAbilityNestingData(EnhanceData.GoodsType);}
        
        // OwnerCharacter->GetRSkillNumber()->AddSkillAbilityNestingData(EnhanceData.GoodsType, AbilityNestingNum);
        // GI->OnSkillEnhanceDataUpdate.Broadcast(ESkillKey::R, OwnerCharacter->GetRSkillNumber()->GetSkillAbilityNestingData());
        // OwnerCharacter->GetRSkillNumber()->UpdateSkillData();
        break;

    case EEnhanceCategory::Enhance_Passive:
        OwnerCharacter->AddPassive_Enhance( EnhanceData.SkillNumber, EnhanceData.GoodsType, AbilityNestingNum.Key, AbilityNestingNum.Value);
        break;
    }

    EnhanceData.GoodsNum--;
    if(ShoppingList[ChoiceNumber]) ShoppingList[ChoiceNumber]->InitEnhanceData(EnhanceData);
    if(ShoppingChoice.IsValid()) ShoppingChoice->InitInformation_Enhance(EnhanceData);
    ShopListData.ShopGoodsList_Enhance[ChoiceNumber] = EnhanceData;
}

void UAPShoppingUI::PurchaseSkill(int32 ChoiceNumber, FShopGoodsData_NewSkill & NewSkillData)
{
    if(!OwnerCharacter.IsValid()) return;
    auto GI = Cast<UAPGameInstance>(GetGameInstance()); if(!GI) return;

    if(ShoppingList[ChoiceNumber]) 
    {
        NewSkillData.bSoldOut = true;
        ShoppingList[ChoiceNumber]->InitNewSkillData(NewSkillData);
    }
    ShopListData.ShopGoodsList_NewSkill[ChoiceNumber - ShopListData.ShopGoodsList_Enhance.Num()] = NewSkillData;
    
    uint8 CurrentSkillNum = 0;
    switch (NewSkillData.GoodsCategory)
    {
        case EEnhanceCategory::Enhance_Q:
        
        OwnerCharacter->SetQSkill((ESkillNumber)NewSkillData.SkillNumber);
        // OwnerCharacter->GetAPSkillHubComponent()->UpdatingSkill_Q();
        OwnerCharacter->GetAPHUD()->GetStatusWidget(OwnerCharacter.Get())->HightLightSkillSlot(ESkillKey::Q);
        GI->OnSkillEnhanceDataClear.Broadcast(ESkillKey::Q);
        OwnerCharacter->SetHavingSkills();
        if(OwnerShop.IsValid()) OwnerShop->SkillChange(ShopListData, EEnhanceCategory::Enhance_Q);
        break;

        case EEnhanceCategory::Enhance_E:
        OwnerCharacter->SetESkill((ESkillNumber)NewSkillData.SkillNumber);
        // OwnerCharacter->GetAPSkillHubComponent()->UpdatingSkill_E();
        OwnerCharacter->GetAPHUD()->GetStatusWidget(OwnerCharacter.Get())->HightLightSkillSlot(ESkillKey::E);
        GI->OnSkillEnhanceDataClear.Broadcast(ESkillKey::E);
        OwnerCharacter->SetHavingSkills();
        if(OwnerShop.IsValid()) OwnerShop->SkillChange(ShopListData, EEnhanceCategory::Enhance_E);
        break;

        case EEnhanceCategory::Enhance_R:
        OwnerCharacter->SetRSkill((EUltSkillNumber)NewSkillData.SkillNumber);
        OwnerCharacter->GetAPHUD()->GetStatusWidget(OwnerCharacter.Get())->HightLightSkillSlot(ESkillKey::R);
        GI->OnSkillEnhanceDataClear.Broadcast(ESkillKey::R);
        OwnerCharacter->SetHavingSkills();
        if(OwnerShop.IsValid()) OwnerShop->SkillChange(ShopListData, EEnhanceCategory::Enhance_R);
        break;

        case EEnhanceCategory::Enhance_Passive:
        CurrentSkillNum = NewSkillData.SkillNumber;
        OwnerCharacter->AddPassive((EPassiveNumber)NewSkillData.SkillNumber);
        break;
    }

    
    if(NewSkillData.GoodsType == EEnHanceType::Gold)
    {
        OwnerCharacter->GetAPHUD()->DisplayEnhanceChoice(NewSkillData.GoodsCategory, EEnHanceType::Silver, true, CurrentSkillNum);
    }
    else if(NewSkillData.GoodsType == EEnHanceType::Platinum)
    {
        OwnerCharacter->GetAPHUD()->DisplayEnhanceChoice(NewSkillData.GoodsCategory, EEnHanceType::Gold, true, CurrentSkillNum);
    }
}

void UAPShoppingUI::OnNotEnoughGold_Implementation()
{
    UAPSoundSubsystem::PlayRejectSound(UAPGameInstance::GetSoundGI(GetWorld()));
}