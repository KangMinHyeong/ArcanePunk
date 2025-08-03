const FItemData* ItemData = DataTableGI->GetRowByStruct<FItemData>(TEXT_Gold, TEXT_Gold.ToString());
auto RowDataTable = DataTableGI->GetRowByStruct<FSkillAbilityRowNameData>(PassiveName, PassiveName.ToString());
auto SilverData = DataTableGI->GetRowByStruct<FSkillAbilityDataSheet>( FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
auto GoldData = DataTableGI->GetRowByStruct<FSkillAbilityDataSheet>( FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
auto PlatinumData = DataTableGI->GetRowByStruct<FSkillAbilityDataSheet>(FName(*RowName[It.Key - 1]), RowName[It.Key - 1]); 