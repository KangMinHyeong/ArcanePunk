void UAPEnhanceChoice::EnhanceSuffle(FName SkillName)
{  
    if(!OwnerCharacter.IsValid()) return; 
    if(!DataTableGI.IsValid()) return;  
    auto DataTable = DataTableGI->GetRowByStruct<FSkillAbilityRowNameData>(SkillName, SkillName.ToString()); if(!DataTable) return;
    
    TArray<FString> RowName;
    uint8 Num = 0;
    TMap<uint8, uint16> AbilityNestingNum;

    switch (EnHanceType)
    {
        case EEnHanceType::Silver:
        RowName = DataTable->SilverRowName;
        Num = RowName.Num(); 
        AbilityNestingNum = SkillAbilityNestingData.SilverAbilityNestingNum;
        break;

        case EEnHanceType::Gold:
        RowName = DataTable->GoldRowName;
        Num = RowName.Num(); 
        AbilityNestingNum = SkillAbilityNestingData.GoldAbilityNestingNum;
        break;

        case EEnHanceType::Platinum:
        RowName = DataTable->PlatinumRowName;
        Num = RowName.Num(); 
        AbilityNestingNum = SkillAbilityNestingData.PlatinumAbilityNestingNum;
        break;
    }

    if(SkillAbilities.IsEmpty())
    {
        for(int32 i = 0; i<Num; i++)
        {
            SkillAbilities.Emplace(i);
        }   

        int32 LastIndex = SkillAbilities.Num()-1;

        for (int32 i = 0; i <= LastIndex; i += 1) 
        {
            int32 Index = FMath::RandRange(i, LastIndex);
            if (i == Index) 
            {
                continue;
            }
            SkillAbilities.Swap(i, Index);
        }
    }
    
    FSkillAbilityDataSheet* AbilityData = nullptr;
    AbilityData = DataTableGI->GetRowByStruct<FSkillAbilityDataSheet>(FName(*RowName[SkillAbilities.Top()]), RowName[SkillAbilities.Top()]);
    if(!AbilityData) return;
    
    
    if(AbilityNestingNum.Contains(SkillAbilities.Top()+1))
    {
        bool Nestable = AbilityData->Nestable;
        if(!Nestable) {SkillAbilities.Pop(); EnhanceSuffle(SkillName);}
        else 
        {
            PlusAbilityNum = AbilityNestingNum[SkillAbilities.Top()+1];
            if(PlusAbilityNum == AbilityData->MaxNesting) {SkillAbilities.Pop(); EnhanceSuffle(SkillName);}
            else {PlusAbilityNum++;}
        }
    }
    else
    {
        PlusAbilityNum = 1;
    }
} 