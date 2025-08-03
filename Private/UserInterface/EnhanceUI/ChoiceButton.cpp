void UChoiceButton::SetEnhance(UUserWidget* UpdateParentWidget, uint8 UpdateSkillNumber, uint8 UpdateSkillAbility ,uint16 UpdateNestingNumb)
{
    bEnhance = true;

    ParentWidget = Cast<UAPEnhanceChoice>(UpdateParentWidget);
    SkillNumber = UpdateSkillNumber;
    SkillAbility = UpdateSkillAbility;
    NestingNumb = UpdateNestingNumb;

    SetSkillName();

    auto DataTableGI = Cast<UAPDataTableSubsystem>(GetGameInstance()->GetSubsystemBase(UAPDataTableSubsystem::StaticClass())); if(!DataTableGI) return;   
    auto DataTable = DataTableGI->GetRowByStruct<FSkillAbilityRowNameData>(SkillNumberName, SkillNumberName.ToString()); if(!DataTable) return;

    FSkillAbilityDataSheet* AbilityData = nullptr;
    TArray<FString> RowName;
    auto OwnerCharacter = Cast<AArcanePunkCharacter>(ParentWidget->GetOwningPlayerPawn()); if(!OwnerCharacter) return;
    
    switch (ParentWidget->GetEnHanceType())
    {
        case EEnHanceType::Silver:
        RowName = DataTable->SilverRowName;
        AbilityData = DataTableGI->GetRowByStruct<FSkillAbilityDataSheet>(FName(*RowName[UpdateSkillAbility]), RowName[UpdateSkillAbility]);
        break;

        case EEnHanceType::Gold:
        RowName = DataTable->GoldRowName;
        AbilityData = DataTableGI->GetRowByStruct<FSkillAbilityDataSheet>(FName(*RowName[UpdateSkillAbility]), RowName[UpdateSkillAbility]);
        break;

        case EEnHanceType::Platinum:
        RowName = DataTable->PlatinumRowName;
        AbilityData = DataTableGI->GetRowByStruct<FSkillAbilityDataSheet>(FName(*RowName[UpdateSkillAbility]), RowName[UpdateSkillAbility]);
        break;
    }
    if(!AbilityData) return;

    MaxNestingNum = AbilityData->MaxNesting;
    if(!AbilityData->Nestable) MaxNestingNum = 1;

    SkillNumber_Text->SetText(FText::FromString(AbilityData->AbilityName));
} 