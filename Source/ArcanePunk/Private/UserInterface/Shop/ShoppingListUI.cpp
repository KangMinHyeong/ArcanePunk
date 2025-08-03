#include "UserInterface/Shop/ShoppingListUI.h"

#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/WidgetSwitcher.h"
#include "UserInterface/Shop/APShoppingUI.h"
#include "DataStructs/Skill/FSkillAbilityDataSheet.h"
#include "DataStructs/Skill/FSkillNameList.h"

void UShoppingListUI::NativeConstruct()
{
    Super::NativeConstruct();

    Button_List->OnClicked.AddDynamic(this, &UShoppingListUI::OnListClick);
}

void UShoppingListUI::InitParent(UUserWidget * UpdateParentWidget, int32 UpdateListNumber)
{
    ParentWidget = UpdateParentWidget;
    ListNumber = UpdateListNumber;
}

void UShoppingListUI::InitEnhanceData(const FShopGoodsData_Enhance & ShopGoodsData_Enhance)
{
    EnhanceData = ShopGoodsData_Enhance;
    if(EnhanceData.GoodsNum == 0) {SoldOut(); return;}
    SwitchingBackgroundColor(EnhanceData.GoodsType);
    SwitchingAbility(EnhanceData.GoodsType, EnhanceData.EnhanceName);
    SetPrice(EnhanceData.GoodsPrice);
    Text_Enhance->SetVisibility(ESlateVisibility::Visible);
    Text_EnhanceName->SetVisibility(ESlateVisibility::Visible);
    Text_GoodsNum->SetText(FText::FromString(FString::FromInt(EnhanceData.GoodsNum)));

    auto DataTable = SkillNameListData->FindRow<FSkillNameList>(EnhanceData.SkillName, EnhanceData.SkillName.ToString()); if(!DataTable) return;
    Text_GoodsName->SetText(FText::FromString(DataTable->SkillName_Korean));

    bEnhanceList = true;
}

void UShoppingListUI::InitNewSkillData(const FShopGoodsData_NewSkill & ShopGoodsData_NewSkill)
{
    NewSkillData = ShopGoodsData_NewSkill;
    if(NewSkillData.bSoldOut) {SoldOut(); return;}
    SwitchingBackgroundColor(NewSkillData.GoodsType);
    SetPrice(NewSkillData.GoodsPrice);
    Text_New->SetVisibility(ESlateVisibility::Visible);

    auto DataTable = SkillNameListData->FindRow<FSkillNameList>(NewSkillData.NewSkillName, NewSkillData.NewSkillName.ToString()); if(!DataTable) return;
    Text_GoodsName->SetText(FText::FromString(DataTable->SkillName_Korean));
    Image_Goods->SetBrushFromTexture(DataTable->SkillSlotImage); 

    bEnhanceList = false;
}

void UShoppingListUI::SoldOut()
{
    Switcher_List->SetActiveWidget(SizeBox_SoldOut);
}

void UShoppingListUI::SwitchingBackgroundColor(EEnHanceType EnHanceType)
{
    switch (EnHanceType)
    {
        case EEnHanceType::Silver:
        Border_Background->SetBrushColor(SilverColor);
    break;
        case EEnHanceType::Gold:
        Border_Background->SetBrushColor(GoldColor);
    break;
        case EEnHanceType::Platinum:
        Border_Background->SetBrushColor(PlatinumColor);
    break;
    }
}

void UShoppingListUI::SetPrice(int32 Price)
{
    Text_GoodsPrice->SetText(FText::FromString(FString::FromInt(Price)));
}

void UShoppingListUI::SwitchingAbility(EEnHanceType EnHanceType, FName AbilityName)
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
    Text_EnhanceName->SetText(FText::FromString(AbilityTable->AbilityName));
    Image_Goods->SetBrushFromTexture(AbilityTable->EnhanceSlotImage); 
}

void UShoppingListUI::OnListClick()
{
    auto Shop = Cast<UAPShoppingUI>(ParentWidget); if(!Shop) return;
    if(bEnhanceList)
    {
        Shop->ClickEnhanceList(EnhanceData, ListNumber);
    }
    else
    {
        Shop->ClickNewSkillList(NewSkillData, ListNumber);
    }
}
