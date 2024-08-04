
#include "UserInterface/Shop/ShoppingChoiceUI.h"

#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "UserInterface/Shop/APShoppingUI.h"
#include "Components/Character/APSkillHubComponent.h"
#include "UserInterface/APHUD.h"
#include "Items/APItemBase.h"
#include "ArcanePunk/Public/Components/APInventoryComponent.h"
#include "GameInstance/APGameInstance.h"
#include "UserInterface/Common/APCheckUI.h"

void UShoppingChoiceUI::NativeConstruct()
{
    Super::NativeConstruct();
    
    Button_Purchase->OnClicked.AddDynamic(this, &UShoppingChoiceUI::OnCheckPurchase);

    APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI.IsValid()) return;  

    APGI->SetTextBlock(Text_New, EStringRowName::NewSkill);
    APGI->SetTextBlock(Text_Type, EStringRowName::TypeName);
    APGI->SetTextBlock(Text_TierName, EStringRowName::TierName);
    APGI->SetTextBlock(Text_PriceName, EStringRowName::Price);
    APGI->SetTextBlock(Text_CurrentGold, EStringRowName::CurrentGold);
    APGI->SetTextBlock(Text_Purchase, EStringRowName::Purchase);
    
}

void UShoppingChoiceUI::InitParent(UUserWidget * UpdateParentWidget, int32 UpdateChoiceNumber)
{
    ParentWidget = UpdateParentWidget;
    ChoiceNumber = UpdateChoiceNumber;
    OwnerCharacter = Cast<AArcanePunkCharacter>(ParentWidget->GetOwningPlayerPawn());
}

void UShoppingChoiceUI::InitInformation_Enhance(FShopGoodsData_Enhance ShopGoodsData_Enhance)
{
    EnhanceData = ShopGoodsData_Enhance;
    if(EnhanceData.GoodsNum == 0) {RemoveFromParent(); return;}
    SwitchingBackgroundColor(EnhanceData.GoodsType);
    SwitchingSkillType(EnhanceData.GoodsCategory);
    SwitchingAbility(EnhanceData.GoodsType, EnhanceData.EnhanceName);
    SwitchingAddition_Enhance(EnhanceData.GoodsCategory, EnhanceData.GoodsType, EnhanceData.EnhanceNumber);
    UpdatePlayerGold(EnhanceData.GoodsPrice);

    APGI->SetTextBlock(Text_GoodsType, EStringRowName::EnhanceSkill);
    Text_Price->SetText(FText::FromString(FString::FromInt(ShopGoodsData_Enhance.GoodsPrice)));

    bEnhanceUI = true;
}

void UShoppingChoiceUI::InitInformation_NewSkill(FShopGoodsData_NewSkill ShopGoodsData_NewSkill)
{
    NewSkillData = ShopGoodsData_NewSkill;
    SwitchingBackgroundColor(NewSkillData.GoodsType);
    SwitchingSkillType(NewSkillData.GoodsCategory);
    SwitchingAddition_NewSkill(NewSkillData.GoodsType);
    UpdatePlayerGold(NewSkillData.GoodsPrice);
    Text_New->SetVisibility(ESlateVisibility::Visible);

    APGI->SetTextBlock(Text_GoodsType, EStringRowName::SkillName);
    Text_Price->SetText(FText::FromString(FString::FromInt(ShopGoodsData_NewSkill.GoodsPrice)));

    auto DataTable = SkillNameListData->FindRow<FSkillNameList>(NewSkillData.NewSkillName, NewSkillData.NewSkillName.ToString()); if(!DataTable) return;
    Text_GoodsName->SetText(FText::FromString(DataTable->SkillName_Korean));
    Text_Information->SetText(FText::FromString(DataTable->Skill_Information));
    Image_Goods->SetBrushFromTexture(DataTable->SkillSlotImage); 

    bEnhanceUI = false;
}

void UShoppingChoiceUI::SwitchingBackgroundColor(EEnHanceType EnHanceType)
{
    APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI.IsValid()) return;  
    auto TierFont = Text_Tier->GetFont();
    auto CategoryFont = Text_Category->GetFont();
    auto GoodsTypeFont = Text_GoodsType->GetFont();
    auto GoodsNameFont = Text_GoodsName->GetFont();
    
    switch (EnHanceType)
    {
        case EEnHanceType::Silver:
        APGI->SetTextBlock(Text_Type, EStringRowName::Tier_3);
        Border_Background->SetBrushColor(SilverColor);
        TierFont.OutlineSettings.OutlineColor = SilverColor;
        CategoryFont.OutlineSettings.OutlineColor = SilverColor;
        GoodsTypeFont.OutlineSettings.OutlineColor = SilverColor;
        GoodsNameFont.OutlineSettings.OutlineColor = SilverColor;
    break;
        case EEnHanceType::Gold:
        APGI->SetTextBlock(Text_Type, EStringRowName::Tier_2);
        Border_Background->SetBrushColor(GoldColor);
        TierFont.OutlineSettings.OutlineColor = GoldColor;
        CategoryFont.OutlineSettings.OutlineColor = GoldColor;
        GoodsTypeFont.OutlineSettings.OutlineColor = GoldColor;
        GoodsNameFont.OutlineSettings.OutlineColor = GoldColor;
    break;
        case EEnHanceType::Platinum:
        APGI->SetTextBlock(Text_Type, EStringRowName::Tier_1);
        Border_Background->SetBrushColor(PlatinumColor);
        TierFont.OutlineSettings.OutlineColor = PlatinumColor;
        CategoryFont.OutlineSettings.OutlineColor = PlatinumColor;
        GoodsTypeFont.OutlineSettings.OutlineColor = PlatinumColor;
        GoodsNameFont.OutlineSettings.OutlineColor = PlatinumColor;
    break;
    }

    Text_Tier->SetFont(TierFont);
    Text_Category->SetFont(CategoryFont);
    Text_GoodsType->SetFont(GoodsTypeFont);
    Text_GoodsName->SetFont(GoodsNameFont);
}

void UShoppingChoiceUI::SwitchingSkillType(EEnhanceCategory EnhanceCategory)
{
    APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI.IsValid()) return;  

    switch (EnhanceCategory)
    {
        case EEnhanceCategory::Enhance_Q:
        APGI->SetTextBlock(Text_Category, EStringRowName::Skill_Q);
    break;
        case EEnhanceCategory::Enhance_E:
        APGI->SetTextBlock(Text_Category, EStringRowName::Skill_E);
    break;
        case EEnhanceCategory::Enhance_R:
        APGI->SetTextBlock(Text_Category, EStringRowName::Skill_R);
    break;
        case EEnhanceCategory::Enhance_Passive:
        APGI->SetTextBlock(Text_Category, EStringRowName::Passive);
    break;
    }
}

void UShoppingChoiceUI::SwitchingAbility(EEnHanceType EnHanceType, FName AbilityName)
{
    FSkillAbilityDataSheet* AbilityTable = nullptr;

    switch (EnHanceType)
    {
        case EEnHanceType::Silver:
        AbilityTable = SkillAbility_Silver->FindRow<FSkillAbilityDataSheet>(AbilityName, AbilityName.ToString()); 
    break;
        case EEnHanceType::Gold:
        AbilityTable = SkillAbility_Gold->FindRow<FSkillAbilityDataSheet>(AbilityName, AbilityName.ToString()); 
    break;
        case EEnHanceType::Platinum:
        AbilityTable = SkillAbility_Platinum->FindRow<FSkillAbilityDataSheet>(AbilityName, AbilityName.ToString()); 
    break;
    }
    if(!AbilityTable) return;
    Text_GoodsName->SetText(FText::FromString(AbilityTable->AbilityName));
    Text_Information->SetText(FText::FromString(AbilityTable->AbilityInformation));
    Image_Goods->SetBrushFromTexture(AbilityTable->EnhanceSlotImage); 
    MaxNum = AbilityTable->MaxNesting;
}

void UShoppingChoiceUI::SwitchingAddition_Enhance(EEnhanceCategory EnhanceCategory, EEnHanceType EnHanceType, uint8 EnhanceNum)
{
    if(!OwnerCharacter.IsValid()) return;
    
    FSkillAbilityNestingData SkillAbilityNestingData;
    switch (EnhanceCategory)
    {
        case EEnhanceCategory::Enhance_Q:
        SkillAbilityNestingData = OwnerCharacter->GetQSkillNumber()->GetSkillAbilityNestingData();
    break;

        case EEnhanceCategory::Enhance_E:
        SkillAbilityNestingData = OwnerCharacter->GetESkillNumber()->GetSkillAbilityNestingData();
    break;

        case EEnhanceCategory::Enhance_R:
        SkillAbilityNestingData = OwnerCharacter->GetRSkillNumber()->GetSkillAbilityNestingData();
    break;

        case EEnhanceCategory::Enhance_Passive:
        if(OwnerCharacter->GetPassiveSkills().Contains(EnhanceData.SkillNumber))
        {
            SkillAbilityNestingData = OwnerCharacter->GetPassiveSkills()[EnhanceData.SkillNumber];
        }
    break;
    }

    TMap<uint8, uint16> AbilityNestingNum;
    switch (EnHanceType)
    {
        case EEnHanceType::Silver:
        AbilityNestingNum = SkillAbilityNestingData.SilverAbilityNestingNum;
    break;

        case EEnHanceType::Gold:
        AbilityNestingNum = SkillAbilityNestingData.GoldAbilityNestingNum;
    break;

        case EEnHanceType::Platinum:
        AbilityNestingNum = SkillAbilityNestingData.PlatinumAbilityNestingNum;
    break;
    }

    uint16 NestingNum = 0;
    if(AbilityNestingNum.Contains(EnhanceNum))
    {
        NestingNum = AbilityNestingNum[EnhanceNum];
    }

    CurrentAbilityData.Key = EnhanceNum;
    CurrentAbilityData.Value = NestingNum;

    
    Text_Addition_Inform->SetText(FText::FromString(FString::FromInt(NestingNum)));

    APGI->SetTextBlock(Text_Addition, EStringRowName::CurrentNesting);
    APGI->SetTextBlock(Text_Addition_Inform_2, EStringRowName::Count);
}

void UShoppingChoiceUI::SwitchingAddition_NewSkill(EEnHanceType EnHanceType)
{
    APGI->SetTextBlock(Text_Addition, EStringRowName::AdditionEnhance);
        
    switch (EnHanceType)
    {
        case EEnHanceType::Silver:
        Text_Addition_1->SetText(FText::FromString(SpaceB));
        Border_Background->SetBrushColor(SilverColor);
    break;
        case EEnHanceType::Gold:
        APGI->SetTextBlock(Text_Addition_Inform_2, EStringRowName::Tier_3);
        Border_Background->SetBrushColor(GoldColor);
    break;
        case EEnHanceType::Platinum:
        APGI->SetTextBlock(Text_Addition_Inform_2, EStringRowName::Tier_2);
        Border_Background->SetBrushColor(PlatinumColor);
    break;
    }
}

void UShoppingChoiceUI::OnValidating(ECheckType UpdateCheckType)
{
    auto Shop = Cast<UAPShoppingUI>(ParentWidget); if(!Shop) return;

    // 재화 체크  
    auto Item = OwnerCharacter->GetInventory()->FindItembyId(TEXT("Gold")); if(!Item) {Shop->OnNotEnoughGold(); return;} // 골드 부족 UI
    if(Item->Quantity >= CurrentPrice)
    {
        OwnerCharacter->GetInventory()->RemoveAmountOfItem(Item, CurrentPrice);
        if(APGI.IsValid()) APGI->OnGettingGold.Broadcast(-CurrentPrice);
    }

    if(bEnhanceUI)
    {
        CurrentAbilityData.Value++;
        OwnerCharacter->GetAPHUD()->DisplayEnhanceGauge(CurrentAbilityData.Value, MaxNum);
        Shop->PurchaseEnhance(ChoiceNumber, EnhanceData, CurrentAbilityData);
    }
    else
    {
        Shop->PurchaseSkill(ChoiceNumber, NewSkillData);
        RemoveFromParent();
    }
}

void UShoppingChoiceUI::OnCheckPurchase()
{
    auto Shop = Cast<UAPShoppingUI>(ParentWidget); if(!Shop) return;

    // 재화 체크  
    auto Item = OwnerCharacter->GetInventory()->FindItembyId(TEXT("Gold")); if(!Item) {Shop->OnNotEnoughGold(); return;} // 골드 부족 UI
    if(Item->Quantity >= CurrentPrice)
    {
        if(!APGI.IsValid()) return;

        auto CheckUI = CreateWidget<UAPCheckUI>(GetWorld(), APGI->GetCheckUIClass());
        if(CheckUI)
        {
            CheckUI->AddToViewport(1); 
            CheckUI->SetCheckType(ECheckType::Purchase, this);
        }
    }
    else
    {
        Shop->OnNotEnoughGold();
    }
}

void UShoppingChoiceUI::UpdatePlayerGold(int32 GoodsPrice)
{
    if(!OwnerCharacter.IsValid()) return;
    CurrentPrice = GoodsPrice;
    int32 PlayerGold = 0;

    auto Item = OwnerCharacter->GetInventory()->FindItembyId(TEXT("Gold")); 
    if(Item){PlayerGold = Item->Quantity;}
    Text_PlayerGold->SetText(FText::FromString(FString::FromInt(PlayerGold)));
    
    if(PlayerGold < CurrentPrice)
    {
        // auto PriceFont = Text_Price->GetFont();
        // PriceFont.OutlineSettings.OutlineColor = RequireColor;
        // Text_Price->SetFont(PriceFont);

        auto PurchaseFont = Text_Purchase->GetFont();
        PurchaseFont.OutlineSettings.OutlineColor = RequireColor;
        Text_Purchase->SetFont(PurchaseFont);
    }

    return;
}