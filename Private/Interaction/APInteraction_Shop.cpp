FSkillAbilityDataSheet* AAPInteraction_Shop::GetTierData(EEnHanceType EnHanceType, const FString & CurrentRowName)
{
    FSkillAbilityDataSheet* Data = nullptr;
    switch (EnHanceType)
    {
    case EEnHanceType::Silver:
        Data = DataTableGI->GetRowByStruct<FSkillAbilityDataSheet>(FName(*CurrentRowName), CurrentRowName);
        break;
    case EEnHanceType::Gold:
        Data = DataTableGI->GetRowByStruct<FSkillAbilityDataSheet>(FName(*CurrentRowName), CurrentRowName);
        break;
    case EEnHanceType::Platinum:
        Data = DataTableGI->GetRowByStruct<FSkillAbilityDataSheet>(FName(*CurrentRowName), CurrentRowName);
        break;
    }
    return Data;
}

void AAPInteraction_Shop::SetEnhanceData(FShopGoodsData_Enhance &EnhanceData, TArray<FString>& RowName, TArray<FString>& AlreadyAbility)
{
    auto DataTable = DataTableGI->GetRowByStruct<FSkillAbilityRowNameData>(EnhanceData.SkillName, EnhanceData.SkillName.ToString()); if(!DataTable) return;
    switch (EnhanceData.GoodsType)
    {
    case EEnHanceType::Silver:
        AlreadyAbility = AlreadyAbility_Silver;
        RowName = DataTable->SilverRowName;
        break;
    case EEnHanceType::Gold:
        AlreadyAbility = AlreadyAbility_Gold;
        RowName = DataTable->GoldRowName;
        break;
    case EEnHanceType::Platinum:
        AlreadyAbility = AlreadyAbility_Platinum;
        RowName = DataTable->PlatinumRowName;
        break;
    }
} 