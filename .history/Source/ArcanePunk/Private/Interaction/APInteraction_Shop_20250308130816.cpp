
#include "Interaction/APInteraction_Shop.h"

#include "PlayerController/ArcanePunkPlayerController.h"
#include "Components/Character/APSkillHubComponent.h"

AAPInteraction_Shop::AAPInteraction_Shop()
{
    ShopMesh =  CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShopMesh"));

	ShopMesh->SetupAttachment(InteractionTrigger);
}

void AAPInteraction_Shop::BeginPlay()
{
    Super::BeginPlay();
    DefaultCategoryPercent = CategoryPercent;
}


void AAPInteraction_Shop::Interact(AArcanePunkCharacter *PlayerCharacter)
{
    Super::Interact(PlayerCharacter);
    if(!PlayerCharacter) return;
    MainPlayer = Cast<AArcanePunkCharacter>(PlayerCharacter);

    // if(MainPlayer->IsDash()) {MainPlayer->ReleasedDash();}

    if(bInit) {bInit = false; InitSuffle();}

    auto PC = Cast<AArcanePunkPlayerController>(MainPlayer->GetController()); if(!PC) return;
    PC->OpenConversationUI(this, DialogoueGroupID);
}

void AAPInteraction_Shop::OpenShopUI()
{
    auto PC = Cast<AArcanePunkPlayerController>(MainPlayer->GetController()); if(!PC) return;
    PC->OpenShoppingUI(this, ShopListData);
}

void AAPInteraction_Shop::InitSuffle()
{
    int32 ListNum = FMath::RandRange(ShopList_Mininum, ShopList_Maximum);

    UE_LOG(LogTemp, Display, TEXT("ListNum : %d"), ListNum);

    ShopListData = {}; // 비우기

    for(int32 i = 0; i<ListNum; i++)
    {
        float Percent = FMath::RandRange(0.0f, 100.0f);
        if(Percent <= SkillPercent)
        {
            AddNewSkill();
        }
        else
        {
            AddEnhance();
        }
    }
    ShopListData.ShopGoodsList_Enhance = Enhance_Spare; Enhance_Spare.Empty();
}

void AAPInteraction_Shop::AddNewSkill()
{
    if(!MainPlayer.IsValid()) return;
    CategoryPercent = DefaultCategoryPercent;
    FShopGoodsData_NewSkill ShopGoodsData_NewSkill = {};

    ShopGoodsData_NewSkill.GoodsCategory = CheckCategoryPercent();
    ShopGoodsData_NewSkill.GoodsType = CheckTierPercent();
    CheckNewSkill(ShopGoodsData_NewSkill);
    
    if(ShopGoodsData_NewSkill.GoodsCategory == EEnhanceCategory::None) return;
    ShopListData.ShopGoodsList_NewSkill.Add(ShopGoodsData_NewSkill);
}

void AAPInteraction_Shop::AddEnhance()
{
    if(!MainPlayer.IsValid()) return;
    CategoryPercent = DefaultCategoryPercent;
    FShopGoodsData_Enhance ShopGoodsData_Enhance = {};

    ShopGoodsData_Enhance.GoodsCategory = CheckCategoryPercent();
    ShopGoodsData_Enhance.GoodsType = CheckTierPercent();
    CheckEnhance(ShopGoodsData_Enhance);

    if(ShopGoodsData_Enhance.GoodsCategory == EEnhanceCategory::None) return;

    uint8 Index = 0;
    for(FShopGoodsData_Enhance& EnhanceData : Enhance_Spare)
    {
        if(EnhanceData.GoodsType == ShopGoodsData_Enhance.GoodsType && EnhanceData.EnhanceName == ShopGoodsData_Enhance.EnhanceName)
        {
            bool Max = CheckEnhanceNumber(EnhanceData); // 맥스에 도달하면 그냥 삭제? 아님 한번 더 뽑기?
            return;
        }
    }
    
    Enhance_Spare.Add(ShopGoodsData_Enhance);
}

bool AAPInteraction_Shop::CheckEnhanceNumber(FShopGoodsData_Enhance & EnhanceData)
{
    auto AbilityData = GetTierData(EnhanceData.GoodsType, EnhanceData.EnhanceName.ToString()); if(!AbilityData) return false;

    uint16 CurrentNesting = 0;
    auto NestingData = GetCurrentNesting(EnhanceData);
    if(NestingData.Contains(EnhanceData.EnhanceNumber)) {CurrentNesting = NestingData[EnhanceData.EnhanceNumber];}

    if(AbilityData->Nestable && EnhanceData.GoodsNum < AbilityData->MaxNesting - CurrentNesting)
    {
        EnhanceData.GoodsNum++;
        return false;
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("Your Nest"));
        AddAlreadyAbility(EnhanceData);
    }
    return true;
}

void AAPInteraction_Shop::AddAlreadyAbility(const FShopGoodsData_Enhance & EnhanceData)
{
    switch (EnhanceData.GoodsType)
    {
    case EEnHanceType::Silver:
        AlreadyAbility_Silver.Add(EnhanceData.EnhanceName.ToString());
        break;
            
    case EEnHanceType::Gold:
        AlreadyAbility_Gold.Add(EnhanceData.EnhanceName.ToString());
        break;
            
    case EEnHanceType::Platinum:
        AlreadyAbility_Platinum.Add(EnhanceData.EnhanceName.ToString());
        break;
    }
}

EEnhanceCategory AAPInteraction_Shop::CheckCategoryPercent()
{
    EEnhanceCategory EnhanceCategory = EEnhanceCategory::None;

    float Passive_Percent = CategoryPercent[EEnhanceCategory::Enhance_Passive];
    float Q_Percent = Passive_Percent + CategoryPercent[EEnhanceCategory::Enhance_Q];
    float E_Percent = Q_Percent + CategoryPercent[EEnhanceCategory::Enhance_E];
    float R_Percent = E_Percent + CategoryPercent[EEnhanceCategory::Enhance_R];
    
    if(R_Percent <= KINDA_SMALL_NUMBER) return EEnhanceCategory::None;

    float CurrentPercent = FMath::RandRange(0.0f, R_Percent);
    
    if(CurrentPercent >= 0.0f && CurrentPercent <= Passive_Percent)
    {
        EnhanceCategory = EEnhanceCategory::Enhance_Passive;
    }
    else if(CurrentPercent > Passive_Percent && CurrentPercent <= Q_Percent)
    {
        EnhanceCategory = EEnhanceCategory::Enhance_Q;
    }
    else if (CurrentPercent > Q_Percent && CurrentPercent <= E_Percent)
    {
        EnhanceCategory = EEnhanceCategory::Enhance_E;
    }
    else
    {
        EnhanceCategory = EEnhanceCategory::Enhance_R;
    }

    return EnhanceCategory;
}

EEnHanceType AAPInteraction_Shop::CheckTierPercent()
{
    EEnHanceType EnHanceType = EEnHanceType::Silver;

    float SilverPercent = TierPercent[EEnHanceType::Silver];
    float GoldPercent = SilverPercent + TierPercent[EEnHanceType::Gold];
    float PlatinumPercent = GoldPercent + TierPercent[EEnHanceType::Platinum];
    float CurrentEnHanceType = FMath::RandRange(0.0f, PlatinumPercent);
    
    if(CurrentEnHanceType >= 0.0f && CurrentEnHanceType <= SilverPercent)
    {
        EnHanceType = EEnHanceType::Silver;
    }
    else if(CurrentEnHanceType > SilverPercent && CurrentEnHanceType <= GoldPercent)
    {
        EnHanceType = EEnHanceType::Gold;
    }
    else if (CurrentEnHanceType > GoldPercent && CurrentEnHanceType <= PlatinumPercent)
    {
        EnHanceType = EEnHanceType::Platinum;
    }

    return EnHanceType;
}

void AAPInteraction_Shop::CheckNewSkill(FShopGoodsData_NewSkill& NewSkillData)
{
    if(!MainPlayer.IsValid()) return;

    if(NewSkillData.GoodsCategory == EEnhanceCategory::Enhance_Q) 
    {
        const UEnum* SkillNum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.EQSkill"), true); if(!SkillNum) return;
        uint8 CurrentNum;

        if(AlreadySkillNumber_Q.Num() == SkillNum->NumEnums() - 2)
        {
            CategoryPercent[EEnhanceCategory::Enhance_Q] = 0.0f;
            NewSkillData.GoodsCategory = CheckCategoryPercent();
            CheckNewSkill(NewSkillData);
            return;
        }
        else
        {
            int loopcnt = 0;
            while(1)
            {
                CurrentNum = FMath::RandRange(1, SkillNum->NumEnums() - 2);
                CurrentNum = SkillNum->GetValueByIndex(CurrentNum);

                if(loopcnt > 100) break;
                loopcnt++;

                if(MainPlayer->GetQSkill() == (ESkillNumber)CurrentNum || AlreadySkillNumber_Q.Contains(CurrentNum)) continue;
                else {break;}
            }
            NewSkillData.SkillKey = ESkillKey::Q;
            NewSkillData.SkillNumber = CurrentNum;
            NewSkillData.NewSkillName = FName(*SkillNum->GetNameStringByValue(CurrentNum));
            AlreadySkillNumber_Q.Add(CurrentNum);
        }
    }
    else if(NewSkillData.GoodsCategory == EEnhanceCategory::Enhance_E) 
    {
        const UEnum* SkillNum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.EESkill"), true); if(!SkillNum) return;
        uint8 CurrentNum;
        
        if(AlreadySkillNumber_E.Num() == SkillNum->NumEnums() - 2)
        {
            CategoryPercent[EEnhanceCategory::Enhance_E] = 0.0f;
            NewSkillData.GoodsCategory = CheckCategoryPercent();
            CheckNewSkill(NewSkillData);
            return;
        }
        else
        {
            int loopcnt = 0;
            while(1)
            {
                CurrentNum = FMath::RandRange(1, SkillNum->NumEnums() - 2);
                CurrentNum = SkillNum->GetValueByIndex(CurrentNum);

                if(loopcnt > 100) break;
                loopcnt++;

                if(MainPlayer->GetESkill() == (ESkillNumber)CurrentNum || AlreadySkillNumber_E.Contains(CurrentNum)) continue;
                else {break;}
            }
            NewSkillData.SkillKey = ESkillKey::E;
            NewSkillData.SkillNumber = CurrentNum;
            NewSkillData.NewSkillName = FName(*SkillNum->GetNameStringByValue(CurrentNum));
            AlreadySkillNumber_E.Add(CurrentNum);
        }
    }
    else if(NewSkillData.GoodsCategory == EEnhanceCategory::Enhance_R) 
    {
        const UEnum* SkillNum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.EUltSkillNumber"), true); if(!SkillNum) return;
        uint8 CurrentNum;

        if(AlreadySkillNumber_R.Num() == SkillNum->NumEnums() - 2)
        {
            CategoryPercent[EEnhanceCategory::Enhance_R] = 0.0f;
            NewSkillData.GoodsCategory = CheckCategoryPercent();
            CheckNewSkill(NewSkillData);
            return;
        }
        else
        {
            int loopcnt = 0;
            while(1)
            {
                CurrentNum = FMath::RandRange(1, SkillNum->NumEnums() - 2);
                CurrentNum = SkillNum->GetValueByIndex(CurrentNum);

                if(loopcnt > 100) break;
                loopcnt++;               

                if(MainPlayer->GetRSkill() == (EUltSkillNumber)CurrentNum || AlreadySkillNumber_R.Contains(CurrentNum)) continue;
                else {break;}
            }
            NewSkillData.SkillKey = ESkillKey::R;
            NewSkillData.SkillNumber = CurrentNum;
            NewSkillData.NewSkillName = FName(*SkillNum->GetNameStringByValue(CurrentNum));
            AlreadySkillNumber_R.Add(CurrentNum);
        }
    }
    else 
    {
        const UEnum* SkillNum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.EPassiveNumber"), true); if(!SkillNum) return;
        uint8 CurrentNum = SkillNum->NumEnums() - 2;
        
        if(CurrentNum == MainPlayer->GetPassiveSkills().Num() || CurrentNum == AlreadySkillNumber_Passive.Num()) // 패시브 모두 획득
        {
            CategoryPercent[EEnhanceCategory::Enhance_Passive] = 0.0f;
            NewSkillData.GoodsCategory = CheckCategoryPercent();
            CheckNewSkill(NewSkillData);
            return;
        }
        else
        {
            int loopcnt = 0;
            while(1)
            {
                if(loopcnt > 100) break;
                loopcnt++;
                
                CurrentNum = FMath::RandRange(1, SkillNum->NumEnums() - 2);

                if(MainPlayer->GetPassiveSkills().Contains(CurrentNum) || AlreadySkillNumber_Passive.Contains(CurrentNum))
                {
                    if(SkillNum->NumEnums() - 2 <= MainPlayer->GetPassiveSkills().Num() + AlreadySkillNumber_Passive.Num()) // 패시브 모두 획득
                    {
                        CategoryPercent[EEnhanceCategory::Enhance_Passive] = 0.0f;
                        NewSkillData.GoodsCategory = CheckCategoryPercent();
                        CheckNewSkill(NewSkillData);
                        return;
                    }
                    else {continue;}
                }
                else {break;}
            }
            NewSkillData.SkillKey = ESkillKey::None;
            NewSkillData.SkillNumber = CurrentNum;
            NewSkillData.NewSkillName = FName(*SkillNum->GetNameStringByValue(CurrentNum));
            AlreadySkillNumber_Passive.Add(CurrentNum); 
        } 
    }

    auto NameData = SkillNameListData->FindRow<FSkillNameList>(NewSkillData.NewSkillName, NewSkillData.NewSkillName.ToString()); if(!NameData) return;
    NewSkillData.GoodsPrice = NameData->Price;
    switch (NewSkillData.GoodsType)
    {
    case EEnHanceType::Gold:
        NewSkillData.GoodsPrice++;
        break;
    
    case EEnHanceType::Platinum:
        NewSkillData.GoodsPrice+=3;
        break;
    }
}

void AAPInteraction_Shop::CheckEnhance(FShopGoodsData_Enhance &EnhanceData)
{
    if(!MainPlayer.IsValid()) return;

    if(EnhanceData.GoodsCategory == EEnhanceCategory::Enhance_Q)  
    {
        const UEnum* SkillNum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.ESkillNumber"), true); if(!SkillNum) return;
        uint8 CurrentNum = (uint8)MainPlayer->GetQSkill();
        EnhanceData.SkillNumber = CurrentNum;
        EnhanceData.SkillName = FName(*SkillNum->GetNameStringByValue(CurrentNum));
    }
    else if(EnhanceData.GoodsCategory == EEnhanceCategory::Enhance_E)
    {
        const UEnum* SkillNum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.ESkillNumber"), true); if(!SkillNum) return;
        uint8 CurrentNum = (uint8)MainPlayer->GetESkill();
        EnhanceData.SkillNumber = CurrentNum;
        EnhanceData.SkillName = FName(*SkillNum->GetNameStringByValue(CurrentNum));
    }
    else if(EnhanceData.GoodsCategory == EEnhanceCategory::Enhance_R)
    {
        const UEnum* SkillNum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.EUltSkillNumber"), true); if(!SkillNum) return;
        uint8 CurrentNum = (uint8)MainPlayer->GetRSkill();
        EnhanceData.SkillNumber = CurrentNum;
        EnhanceData.SkillName = FName(*SkillNum->GetNameStringByValue(CurrentNum));
    }
    else
    {
        if(MainPlayer->GetPassiveSkills().Num() == 0) {AddEnhance(); EnhanceData.GoodsCategory = EEnhanceCategory::None; return;}
        const UEnum* SkillNum = FindObject<UEnum>(nullptr, TEXT("/Script/ArcanePunk.EPassiveNumber"), true); if(!SkillNum) return;
        uint8 CurrentNum = 0;

        TArray<uint8> PassiveSkillKeys;
        for(auto It : MainPlayer->GetPassiveSkills())
        {
            PassiveSkillKeys.Add(It.Key);
        }
        CurrentNum = FMath::RandRange(0, PassiveSkillKeys.Num() - 1);

        EnhanceData.SkillNumber = PassiveSkillKeys[CurrentNum];
        EnhanceData.SkillName = FName(*SkillNum->GetNameStringByValue(EnhanceData.SkillNumber));
    }

    TArray<FString> RowName; 
    TArray<FString> AlreadyAbility;
    SetEnhanceData(EnhanceData, RowName, AlreadyAbility);

    TMap<uint8, uint16> AbilityNestingNum = GetCurrentNesting(EnhanceData);
        
    TArray<uint8> SkillAbilities = IndexSuffle(RowName.Num());

    FString CurrentRowName;
    int loopcnt = 0;
    while(1)
    {
        if(loopcnt > 100) break;
        loopcnt++;
        if(SkillAbilities.IsEmpty())
        {
            EnhanceData.GoodsCategory = EEnhanceCategory::None; return;
        }

        CurrentRowName = RowName[SkillAbilities.Top()];
        if(AlreadyAbility.Contains(CurrentRowName))
        {
            SkillAbilities.Pop(); continue;
        } 
        else{break;}
    }
    EnhanceData.EnhanceNumber = SkillAbilities.Top() + 1;
    EnhanceData.EnhanceName = FName(*CurrentRowName);

    auto AbilityData = GetTierData(EnhanceData.GoodsType, CurrentRowName);

    EnhanceData.GoodsPrice = AbilityData->Price;
    if(AbilityNestingNum.Contains(EnhanceData.EnhanceNumber))
    {
        if(!AbilityData->Nestable || AbilityNestingNum[EnhanceData.EnhanceNumber] >= AbilityData->MaxNesting) 
        {
            AddAlreadyAbility(EnhanceData);
            SkillAbilities.Pop();
            CheckEnhance(EnhanceData);
        }
    }
}

TArray<uint8> AAPInteraction_Shop::IndexSuffle(uint8 MaxNumber)
{
    TArray<uint8> SkillAbilities;
    if(SkillAbilities.IsEmpty())
    {
        for(int32 i = 0; i<MaxNumber; i++)
        {
           SkillAbilities.Add(i);
        }   

        int32 LastIndex = MaxNumber-1;

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

    return SkillAbilities;
}

FSkillAbilityDataSheet* AAPInteraction_Shop::GetTierData(EEnHanceType EnHanceType, const FString & CurrentRowName)
{
    FSkillAbilityDataSheet* Data = nullptr;
    switch (EnHanceType)
    {
    case EEnHanceType::Silver:
        Data = SkillAbility_Silver->FindRow<FSkillAbilityDataSheet>(FName(*CurrentRowName), CurrentRowName);
        break;
        
    case EEnHanceType::Gold:
        Data = SkillAbility_Gold->FindRow<FSkillAbilityDataSheet>(FName(*CurrentRowName), CurrentRowName);
        break;
        
    case EEnHanceType::Platinum:
        Data = SkillAbility_Platinum->FindRow<FSkillAbilityDataSheet>(FName(*CurrentRowName), CurrentRowName);
        break;
    }
    return Data;
}

void AAPInteraction_Shop::SetEnhanceData(FShopGoodsData_Enhance &EnhanceData, TArray<FString>& RowName, TArray<FString>& AlreadyAbility)
{
    auto DataTable = SkillRowNameData->FindRow<FSkillAbilityRowNameData>(EnhanceData.SkillName, EnhanceData.SkillName.ToString()); if(!DataTable) return;
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

TMap<uint8, uint16> AAPInteraction_Shop::GetCurrentNesting(const FShopGoodsData_Enhance & EnhanceData)
{
    TMap<uint8, uint16> AbilityNestingNum;
    switch (EnhanceData.GoodsType)
    {
    case EEnHanceType::Silver:

        // if(EnhanceData.GoodsCategory == EEnhanceCategory::Enhance_Q)
        // {AbilityNestingNum = MainPlayer->GetQSkillNumber()->GetSkillAbilityNestingData().SilverAbilityNestingNum;}
        // else if(EnhanceData.GoodsCategory == EEnhanceCategory::Enhance_E)
        // {AbilityNestingNum = MainPlayer->GetESkillNumber()->GetSkillAbilityNestingData().SilverAbilityNestingNum;}
        // else if(EnhanceData.GoodsCategory == EEnhanceCategory::Enhance_R)
        // {AbilityNestingNum = MainPlayer->GetRSkillNumber()->GetSkillAbilityNestingData().SilverAbilityNestingNum;}
        break;
        
    case EEnHanceType::Gold:

        if(EnhanceData.GoodsCategory == EEnhanceCategory::Enhance_Q)
        {AbilityNestingNum = MainPlayer->GetQSkillNumber()->GetSkillAbilityNestingData().GoldAbilityNestingNum;}
        else if(EnhanceData.GoodsCategory == EEnhanceCategory::Enhance_E)
        {AbilityNestingNum = MainPlayer->GetESkillNumber()->GetSkillAbilityNestingData().GoldAbilityNestingNum;}
        else if(EnhanceData.GoodsCategory == EEnhanceCategory::Enhance_R)
        {AbilityNestingNum = MainPlayer->GetRSkillNumber()->GetSkillAbilityNestingData().GoldAbilityNestingNum;}
        
        break;
        
    case EEnHanceType::Platinum:

        if(EnhanceData.GoodsCategory == EEnhanceCategory::Enhance_Q)
        {AbilityNestingNum = MainPlayer->GetQSkillNumber()->GetSkillAbilityNestingData().PlatinumAbilityNestingNum;}
        else if(EnhanceData.GoodsCategory == EEnhanceCategory::Enhance_E)
        {AbilityNestingNum = MainPlayer->GetESkillNumber()->GetSkillAbilityNestingData().PlatinumAbilityNestingNum;}
        else if(EnhanceData.GoodsCategory == EEnhanceCategory::Enhance_R)
        {AbilityNestingNum = MainPlayer->GetRSkillNumber()->GetSkillAbilityNestingData().PlatinumAbilityNestingNum;}
        
        break;
    }
    return AbilityNestingNum;
}

void AAPInteraction_Shop::SkillChange(FShopListData ShopData, EEnhanceCategory ChangedSkill)
{
    if(!MainPlayer.IsValid()) return;

    uint16 BeforeNum = ShopData.ShopGoodsList_Enhance.Num();
    ShopData.ShopGoodsList_Enhance.RemoveAll([ChangedSkill](FShopGoodsData_Enhance EnhanceData) 
    {
        return EnhanceData.GoodsCategory == ChangedSkill;
    });
    ShopListData = ShopData;

    uint16 AfterNum = BeforeNum - ShopData.ShopGoodsList_Enhance.Num();
    
    UE_LOG(LogTemp, Display, TEXT("Your %d"), AfterNum);

    for(int32 i = 0; i<AfterNum; i++)
    {
        AddEnhance();
    }

    for( auto Enhance : Enhance_Spare )
    {
        ShopListData.ShopGoodsList_Enhance.Emplace(Enhance);
    }
    Enhance_Spare.Empty();

    auto PC = Cast<AArcanePunkPlayerController>(MainPlayer->GetController()); if(!PC) return;
    PC->CloseShoppingUI();
    PC->OpenShoppingUI(this, ShopListData);
}
