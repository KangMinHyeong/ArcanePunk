#include "UserInterface/Shop/ShoppingChoiceUI.h"

#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "UserInterface/Shop/APShoppingUI.h"
#include "Components/Character/APSkillHubComponent.h"
#include "UserInterface/HUD/APHUD.h"
#include "Items/APItemBase.h"
#include "ArcanePunk/Public/Components/Character/APInventoryComponent.h"
#include "GameInstance/APGameInstance.h"
#include "UserInterface/Common/Check/APCheckUI.h"
#include "DataStructs/Skill/FSkillAbilityData.h"
#include "DataStructs/Skill/FSkillNameList.h"

void UShoppingChoiceUI::NativeConstruct()
{
    Super::NativeConstruct();
    
    Button_Purchase->OnClicked.AddDynamic(this, &UShoppingChoiceUI::OnCheckPurchase);
    Button_Purchase->OnHovered.AddDynamic(UAPGameInstance::GetSoundGI(GetWorld()), &UAPSoundSubsystem::PlayUIHoverSound);

    APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI.IsValid()) return;

    
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_New, EStringRowName::NewSkill);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_Type, EStringRowName::TypeName);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_TierName, EStringRowName::TierName);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_PriceName, EStringRowName::Price);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_CurrentGold, EStringRowName::CurrentGold);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_Purchase, EStringRowName::Purchase);
   
}

void UShoppingChoiceUI::InitParent(UUserWidget * UpdateParentWidget, int32 UpdateChoiceNumber)
{
    ParentWidget = UpdateParentWidget;
    ChoiceNumber = UpdateChoiceNumber;
    OwnerCharacter = Cast<AArcanePunkCharacter>(ParentWidget->GetOwningPlayerPawn());
}

void UShoppingChoiceUI::InitInformation_Enhance(const FShopGoodsData_Enhance & ShopGoodsData_Enhance)
{
    EnhanceData = ShopGoodsData_Enhance;
    if(EnhanceData.GoodsNum == 0) {RemoveFromParent(); return;}
    SwitchingBackgroundColor(EnhanceData.GoodsType);
    SwitchingSkillType(EnhanceData.GoodsCategory);
    SwitchingAbility(EnhanceData.GoodsType, EnhanceData.EnhanceName);
    SwitchingAddition_Enhance(EnhanceData.GoodsCategory, EnhanceData.GoodsType, EnhanceData.EnhanceNumber);
    SwitchingTier(EnhanceData.GoodsType);
    UpdatePlayerGold(EnhanceData.GoodsPrice);

    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_GoodsType, EStringRowName::EnhanceSkill);
    Text_Price->SetText(FText::FromString(FString::FromInt(ShopGoodsData_Enhance.GoodsPrice))); 

    bEnhanceUI = true;
}

void UShoppingChoiceUI::InitInformation_NewSkill(const FShopGoodsData_NewSkill & ShopGoodsData_NewSkill)
{
    NewSkillData = ShopGoodsData_NewSkill;
    SwitchingBackgroundColor(NewSkillData.GoodsType);
    SwitchingSkillType(NewSkillData.GoodsCategory);
    SwitchingAddition_NewSkill(NewSkillData.GoodsType);
    SwitchingTier(NewSkillData.GoodsType);
    UpdatePlayerGold(NewSkillData.GoodsPrice);
    Text_New->SetVisibility(ESlateVisibility::Visible);

    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_GoodsType, EStringRowName::SkillName);
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
        
        Border_Background->SetBrushColor(SilverColor);
        TierFont.OutlineSettings.OutlineColor = SilverColor;
        CategoryFont.OutlineSettings.OutlineColor = SilverColor;
        GoodsTypeFont.OutlineSettings.OutlineColor = SilverColor;
        GoodsNameFont.OutlineSettings.OutlineColor = SilverColor;
    break;
        case EEnHanceType::Gold:
        
        Border_Background->SetBrushColor(GoldColor);
        TierFont.OutlineSettings.OutlineColor = GoldColor;
        CategoryFont.OutlineSettings.OutlineColor = GoldColor;
        GoodsTypeFont.OutlineSettings.OutlineColor = GoldColor;
        GoodsNameFont.OutlineSettings.OutlineColor = GoldColor;
    break;
        case EEnHanceType::Platinum:
        
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
        UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_Category, EStringRowName::Skill_Q);
    break;
        case EEnhanceCategory::Enhance_E:
        UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_Category, EStringRowName::Skill_E);
    break;
        case EEnhanceCategory::Enhance_R:
        UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_Category, EStringRowName::Skill_R);
    break;
        case EEnhanceCategory::Enhance_Passive:
        UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_Category, EStringRowName::Passive);
    break;
    }
}

void UShoppingChoiceUI::SwitchingAbility(EEnHanceType EnHanceType, FName AbilityName)
{
    FSkillAbilityData* AbilityTable = nullptr;

    switch (EnHanceType)
    {
        case EEnHanceType::Silver:
        AbilityTable = SkillAbility_Silver->FindRow<FSkillAbilityData>(AbilityName, AbilityName.ToString()); 
    break;
        case EEnHanceType::Gold:
        AbilityTable = SkillAbility_Gold->FindRow<FSkillAbilityData>(AbilityName, AbilityName.ToString()); 
    break;
        case EEnHanceType::Platinum:
        AbilityTable = SkillAbility_Platinum->FindRow<FSkillAbilityData>(AbilityName, AbilityName.ToString()); 
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
        // SkillAbilityNestingData = OwnerCharacter->GetQSkillNumber()->GetSkillAbilityNestingData();
    break;

        case EEnhanceCategory::Enhance_E:
        // SkillAbilityNestingData = OwnerCharacter->GetESkillNumber()->GetSkillAbilityNestingData();
    break;

        case EEnhanceCategory::Enhance_R:
        // SkillAbilityNestingData = OwnerCharacter->GetRSkillNumber()->GetSkillAbilityNestingData();
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

    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_Addition, EStringRowName::CurrentNesting);
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_Addition_Inform_2, EStringRowName::Count);
}

void UShoppingChoiceUI::SwitchingAddition_NewSkill(EEnHanceType EnHanceType)
{
    UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_Addition, EStringRowName::AdditionEnhance);
        
    switch (EnHanceType)
    {
        case EEnHanceType::Silver:
        Text_Addition_1->SetText(FText::FromString(SpaceB));
        Border_Background->SetBrushColor(SilverColor);
    break;
        case EEnHanceType::Gold:
        UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_Addition_Inform_2, EStringRowName::Tier_3);
        Border_Background->SetBrushColor(GoldColor);
    break;
        case EEnHanceType::Platinum:
        UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_Addition_Inform_2, EStringRowName::Tier_2);
        Border_Background->SetBrushColor(PlatinumColor);
    break;
    }
}

void UShoppingChoiceUI::SwitchingTier(EEnHanceType EnHanceType)
{
    switch (EnHanceType)
    {
        case EEnHanceType::Silver:
        UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_Tier, EStringRowName::Tier_3);
    break;
        case EEnHanceType::Gold:
        UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_Tier, EStringRowName::Tier_2);

    break;
        case EEnHanceType::Platinum:
        UAPDataTableSubsystem::SetTextBlock(UAPGameInstance::GetDataTableGI(GetWorld()), Text_Tier, EStringRowName::Tier_1);
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

        UAPSoundSubsystem::PlayUIChoiceSound(UAPGameInstance::GetSoundGI(GetWorld()));
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
        auto DataTableGI = Cast<UAPDataTableSubsystem>(APGI->GetSubsystemBase(UAPDataTableSubsystem::StaticClass())); if(!DataTableGI) return;     
        auto CheckUI = CreateWidget<UAPCheckUI>(GetWorld(), DataTableGI->GetCheckUIClass());
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