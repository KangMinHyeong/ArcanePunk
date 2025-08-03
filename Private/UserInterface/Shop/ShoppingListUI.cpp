void UShoppingListUI::SwitchingAbility(EEnHanceType EnHanceType, FName AbilityName)
{
    FSkillAbilityDataSheet* AbilityTable = nullptr;

    AbilityTable = DataTableGI->GetRowByStruct<FSkillAbilityDataSheet>(AbilityName, AbilityName.ToString());
    if(!AbilityTable) return;
    Text_EnhanceName->SetText(FText::FromString(AbilityTable->AbilityName));
    Image_Goods->SetBrushFromTexture(AbilityTable->EnhanceSlotImage); 
} 