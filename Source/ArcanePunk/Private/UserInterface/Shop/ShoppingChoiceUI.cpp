
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

void UShoppingChoiceUI::NativeConstruct()
{
    Super::NativeConstruct();
    
    Button_Purchase->OnClicked.AddDynamic(this, &UShoppingChoiceUI::OnCheckPurchase);
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

    FString GoodsType = TEXT("스킬 강화 - ");
    Text_GoodsType->SetText(FText::FromString(GoodsType));
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

    FString GoodsType = TEXT("스킬 이름 : ");
    Text_GoodsType->SetText(FText::FromString(GoodsType));
    Text_Price->SetText(FText::FromString(FString::FromInt(ShopGoodsData_NewSkill.GoodsPrice)));

    auto DataTable = SkillNameListData->FindRow<FSkillNameList>(NewSkillData.NewSkillName, NewSkillData.NewSkillName.ToString()); if(!DataTable) return;
    Text_GoodsName->SetText(FText::FromString(DataTable->SkillName_Korean));
    Text_Information->SetText(FText::FromString(DataTable->Skill_Information));
    Image_Goods->SetBrushFromTexture(DataTable->SkillSlotImage); 

    bEnhanceUI = false;
}

void UShoppingChoiceUI::SwitchingBackgroundColor(EEnHanceType EnHanceType)
{
    auto TierFont = Text_Tier->GetFont();
    auto CategoryFont = Text_Category->GetFont();
    auto GoodsTypeFont = Text_GoodsType->GetFont();
    auto GoodsNameFont = Text_GoodsName->GetFont();

    FString ColorName;
    switch (EnHanceType)
    {
        case EEnHanceType::Silver:
        ColorName = TEXT("티어 : 실버");
        Border_Background->SetBrushColor(SilverColor);
        TierFont.OutlineSettings.OutlineColor = SilverColor;
        CategoryFont.OutlineSettings.OutlineColor = SilverColor;
        GoodsTypeFont.OutlineSettings.OutlineColor = SilverColor;
        GoodsNameFont.OutlineSettings.OutlineColor = SilverColor;
    break;
        case EEnHanceType::Gold:
        ColorName = TEXT("티어 : 골드");
        Border_Background->SetBrushColor(GoldColor);
        TierFont.OutlineSettings.OutlineColor = GoldColor;
        CategoryFont.OutlineSettings.OutlineColor = GoldColor;
        GoodsTypeFont.OutlineSettings.OutlineColor = GoldColor;
        GoodsNameFont.OutlineSettings.OutlineColor = GoldColor;
    break;
        case EEnHanceType::Platinum:
        ColorName = TEXT("티어 : 플레티넘");
        Border_Background->SetBrushColor(PlatinumColor);
        TierFont.OutlineSettings.OutlineColor = PlatinumColor;
        CategoryFont.OutlineSettings.OutlineColor = PlatinumColor;
        GoodsTypeFont.OutlineSettings.OutlineColor = PlatinumColor;
        GoodsNameFont.OutlineSettings.OutlineColor = PlatinumColor;
    break;
    }
    Text_Tier->SetText(FText::FromString(ColorName));
    Text_Tier->SetFont(TierFont);
    Text_Category->SetFont(CategoryFont);
    Text_GoodsType->SetFont(GoodsTypeFont);
    Text_GoodsName->SetFont(GoodsNameFont);
}

void UShoppingChoiceUI::SwitchingSkillType(EEnhanceCategory EnhanceCategory)
{
    FString SkillCategoryName;
    switch (EnhanceCategory)
    {
        case EEnhanceCategory::Enhance_Q:
        SkillCategoryName = TEXT("타입 : Q스킬");
    break;
        case EEnhanceCategory::Enhance_E:
        SkillCategoryName = TEXT("타입 : E스킬");
    break;
        case EEnhanceCategory::Enhance_R:
        SkillCategoryName = TEXT("타입 : R스킬");
    break;
        case EEnhanceCategory::Enhance_Passive:
        SkillCategoryName = TEXT("타입 : 패시브");
    break;
    }
    Text_Category->SetText(FText::FromString(SkillCategoryName));
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
    FString AdditionName = TEXT("현재 중첩 : ");
    FString Addition_Inform_2 = TEXT(" 회");
    
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

    Text_Addition->SetText(FText::FromString(AdditionName));
    Text_Addition_Inform->SetText(FText::FromString(FString::FromInt(NestingNum)));
    Text_Addition_Inform_2->SetText(FText::FromString(Addition_Inform_2));
}

void UShoppingChoiceUI::SwitchingAddition_NewSkill(EEnHanceType EnHanceType)
{
    FString AdditionName;
    switch (EnHanceType)
    {
        case EEnHanceType::Silver:
        AdditionName = TEXT("추가 증강 : X");
        Border_Background->SetBrushColor(SilverColor);
    break;
        case EEnHanceType::Gold:
        AdditionName = TEXT("추가 증강 : 실버");
        Border_Background->SetBrushColor(GoldColor);
    break;
        case EEnHanceType::Platinum:
        AdditionName = TEXT("추가 증강 : 골드");
        Border_Background->SetBrushColor(PlatinumColor);
    break;
    }
    Text_Addition->SetText(FText::FromString(AdditionName));
}

void UShoppingChoiceUI::OnCheckPurchase()
{
    auto Shop = Cast<UAPShoppingUI>(ParentWidget); if(!Shop) return;

    // 재화 체크  
    auto Item = OwnerCharacter->GetInventory()->FindItembyId(TEXT("Gold")); if(!Item) {Shop->OnNotEnoughGold(); return;} // 골드 부족 UI
    if(Item->Quantity >= CurrentPrice)
    {
        OwnerCharacter->GetInventory()->RemoveAmountOfItem(Item, CurrentPrice);
        auto APGI = Cast<UAPGameInstance>(GetGameInstance()); 
        if(APGI) APGI->OnGettingGold.Broadcast(-CurrentPrice);
    }
    else
    {
        Shop->OnNotEnoughGold();
        return;
    }
    
    if(bEnhanceUI)
    {
        CurrentAbilityData.Value++;
        OwnerCharacter->GetAPHUD()->DisplayEnhanceGauge(CurrentAbilityData.Value, MaxNum);
        UE_LOG(LogTemp, Display, TEXT("Your %d, %d"), CurrentAbilityData.Value, MaxNum);
        Shop->PurchaseEnhance(ChoiceNumber, EnhanceData, CurrentAbilityData);
    }
    else
    {
        Shop->PurchaseSkill(ChoiceNumber, NewSkillData);
        RemoveFromParent();
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