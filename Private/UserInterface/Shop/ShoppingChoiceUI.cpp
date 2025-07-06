void UShoppingChoiceUI::SwitchingAbility(EEnHanceType EnHanceType, FName AbilityName)
{
    FSkillAbilityDataSheet* AbilityTable = nullptr;

    AbilityTable = DataTableGI->GetRowByStruct<FSkillAbilityDataSheet>(AbilityName, AbilityName.ToString());
    if(!AbilityTable) return;
    Text_GoodsName->SetText(FText::FromString(AbilityTable->AbilityName));
    Text_Information->SetText(FText::FromString(AbilityTable->AbilityInformation));
    Image_Goods->SetBrushFromTexture(AbilityTable->EnhanceSlotImage); 
    MaxNum = AbilityTable->MaxNesting;
} 