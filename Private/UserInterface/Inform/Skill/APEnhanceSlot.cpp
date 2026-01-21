void UAPEnhanceSlot::InitEnhanceData(UUserWidget *Parent, EEnHanceType UpdateEnhanceType, const FName & UpdateRowName, uint8 UpdateEnhanceNumber, uint16 UpdateNestingNumber)
{
    ParentWidget = Parent;
    EnHanceType = UpdateEnhanceType;
    EnhanceNumber = UpdateEnhanceNumber;
    NestingNumber = UpdateNestingNumber;

    auto DataTable = DataTableGI->GetRowByStruct<FSkillAbilityRowNameData>(UpdateRowName, UpdateRowName.ToString()); if(!DataTable) return;
    switch (EnHanceType)
    {
    case EEnHanceType::Silver:
        RowName = DataTable->SilverRowName[EnhanceNumber-1];
        AbilityData = DataTableGI->GetRowByStruct<FSkillAbilityDataSheet>(FName(*RowName), RowName); 
        break;
    case EEnHanceType::Gold:
        RowName = DataTable->GoldRowName[EnhanceNumber-1];
        AbilityData = DataTableGI->GetRowByStruct<FSkillAbilityDataSheet>(FName(*RowName), RowName); 
        Image_TierColor->SetColorAndOpacity(GoldColor);
        break;
    case EEnHanceType::Platinum:
        RowName = DataTable->PlatinumRowName[EnhanceNumber-1];
        AbilityData = DataTableGI->GetRowByStruct<FSkillAbilityDataSheet>(FName(*RowName), RowName); 
        Image_TierColor->SetColorAndOpacity(PlatinumColor);
        break;
    }
    if(!AbilityData) return;
    auto EnhanceImage = AbilityData->EnhanceSlotImage;

    auto ButtonStyle = Button_Enhance->GetStyle(); 
    ButtonStyle.Normal.SetResourceObject(EnhanceImage);
    ButtonStyle.Hovered.SetResourceObject(EnhanceImage);
    ButtonStyle.Pressed.SetResourceObject(EnhanceImage);
    Button_Enhance->SetStyle(ButtonStyle); 
    // RowName 바꾸기 Skill_8 -> Skill_8_1
    // image 바꾸기
} 