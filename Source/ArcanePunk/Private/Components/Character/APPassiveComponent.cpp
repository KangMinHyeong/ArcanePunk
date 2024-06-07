
#include "Components/Character/APPassiveComponent.h"

#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APSkillHubComponent.h"
#include "Components/SkillActor/APSkillAbility.h"
#include "UserInterface/APHUD.h"
#include "Components/Common/APBuffComponent.h"
#include "Components/Common/APCrowdControlComponent.h"
#include "GameInstance/APGameInstance.h"
#include "ArcanePunk/Public/Components/APInventoryComponent.h"
#include "Items/APItemBase.h"
#include "Components/Character/APAttackComponent.h"

UAPPassiveComponent::UAPPassiveComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UAPPassiveComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UAPPassiveComponent::InitPassive()
{
	OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner()); if(!OwnerCharacter.IsValid()) return;
	Status_Init =  OwnerCharacter->GetPlayerStatus_Origin();
	MPRecoveryTime_Init = OwnerCharacter->GetAPSkillHubComponent()->GetRecoveryTime_MP();
	UpdateFromGameInstance();
	

	for( auto PassiveSkill : OwnerCharacter->GetPassiveSkills())
	{
		ApplyNewPassive((EPassiveNumber)PassiveSkill.Key);
	}
}

void UAPPassiveComponent::UpdateFromGameInstance()
{
	APGI = Cast<UAPGameInstance>(OwnerCharacter->GetGameInstance()); if(!APGI.IsValid()) return;

	if(APGI->GetPackageDropMap().Contains(TEXT_Gold))
	{
		GoldPercent_Init = APGI->GetPackageDropMap()[TEXT_Gold];
	}

	APGI->OnGettingGold.AddUObject(this, &UAPPassiveComponent::ApplyMoneyPower);
	APGI->OnGettingGold.AddUObject(this, &UAPPassiveComponent::RecoveryHP);
}


void UAPPassiveComponent::CheckDamagedGold()
{
	float Check = FMath::RandRange(0.0f, 100.0f);
	if(Check <= DamagedGoldPercent)
	{
		const FItemData* ItemData = APGI->GetEquipDataTable()->FindRow<FItemData>(TEXT_Gold, TEXT_Gold.ToString());

		auto AddItemReference = NewObject<UAPItemBase>(this, UAPItemBase::StaticClass());
		AddItemReference->ID = ItemData->ID;
		AddItemReference->ItemType = ItemData->ItemType;
		AddItemReference->ItemQuality = ItemData->ItemQuality;
		AddItemReference->ItemNumericData = ItemData->ItemNumericData;
		AddItemReference->ItemTextData = ItemData->ItemTextData;
		AddItemReference->ItemAssetData = ItemData->ItemAssetData;
		AddItemReference->ItemStatistics = ItemData->ItemStatistics;

		int32 Amount = APGI->CheckGoldAmount();
		AddItemReference->SetQuantity(Amount);	

		OwnerCharacter->GetInventory()->HandleAddItem(AddItemReference);
	}
}

void UAPPassiveComponent::ApplyMoneyPower(int32 AddNum)
{
	if(!MoneyPower) return;
	float ATK = OwnerCharacter->GetDefaultATK_Origin();
	ATK = ATK + MoneyPowerPercent * AddNum * 100.0f;
	
	OwnerCharacter->SetDefaultATK_Origin(ATK);
}

void UAPPassiveComponent::RecoveryHP(int32 AddNum)
{
	if(!MoneyRecovery) return;

	float Check = FMath::RandRange(0.0f, 100.0f);
	if(Check <= HPRecoveryPercent)
	{
		auto PDD = OwnerCharacter->GetPlayerStatus(); float OriginHP = PDD.PlayerDynamicData.HP;
		float HP = PDD.PlayerDynamicData.HP + PDD.PlayerDynamicData.MaxHP * HPRecoveryAmount; // 최대체력 비례
		PDD.PlayerDynamicData.HP = FMath::Min(PDD.PlayerDynamicData.MaxHP, HP);

		OwnerCharacter->SetDefaultHP(PDD.PlayerDynamicData.HP); 
		OwnerCharacter->GetAPHUD()->OnUpdateHPBar.Broadcast(OriginHP);
	}
}

void UAPPassiveComponent::ApplyNewPassive(EPassiveNumber PassiveNum)
{
	if(!OwnerCharacter.IsValid()) return;

	switch (PassiveNum)
	{
	case EPassiveNumber::Passive_1: // 최대체력 증가
		UpdateMaxHP();
		break;
		
	case EPassiveNumber::Passive_2: // 공격력 증가
		UpdateATK();
		break;

	case EPassiveNumber::Passive_3: // 공격속도 증가
		UpdateATKSpeed();
		break;
		
	case EPassiveNumber::Passive_4: // 이동속도 증가
		UpdateMoveSpeed();
		break;

	case EPassiveNumber::Passive_5: // MP 증강
		UpdateMaxMP();
		break;

	case EPassiveNumber::Passive_6: // 치명타 확률 증가
		UpdateCriticalPercent();
		break;

	case EPassiveNumber::Passive_7: // 방어력 증가
		UpdateDEF();
		break;

	case EPassiveNumber::Passive_8: // 골드 획득률 증가
		UpdateGoldPercent();
		break;

	case EPassiveNumber::Passive_9: // 골드 획득률 증가
		UpdateDamaged();
		break;
	
	case EPassiveNumber::Passive_10: // 골드 획득률 증가
		UpdateDrain();
		break;
	}
}

void UAPPassiveComponent::UpdateMaxHP()
{
	auto MaxHP = Status_Init.PlayerDynamicData.MaxHP;

	float HPPlus = 20.0f;
	float HPCoeff = 0.1f; 	// 기본 증가 // 하드코딩
	
	// 증강 증가
	FName PassiveName = TEXT("Passive_1");
	auto RowDataTable = APGI->GetSkillAbilityRowData()->FindRow<FSkillAbilityRowNameData>(PassiveName, PassiveName.ToString());
	auto PassiveNestingData = OwnerCharacter->GetPassiveSkills()[(uint8)EPassiveNumber::Passive_1];

	auto RowName = RowDataTable->SilverRowName;
	for(auto It : PassiveNestingData.SilverAbilityNestingNum)
    {
		auto SilverData = APGI->GetSilverAbilityDataTable()->FindRow<FSkillAbilityDataSheet>( FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
		if(!SilverData) return;
		switch (It.Key)
		{
			case 1: // MaxHP 증가 , 합연산
			HPPlus = HPPlus + SilverData->Coefficient_X * It.Value;
			break;

			case 2: // MaxHP 증가 , 곱연산
			HPCoeff = HPCoeff + SilverData->Coefficient_X * It.Value;
			break;
		}
    }
	RowName = RowDataTable->GoldRowName;
    for(auto It : PassiveNestingData.GoldAbilityNestingNum)
    {
		auto GoldData = APGI->GetGoldAbilityDataTable()->FindRow<FSkillAbilityDataSheet>( FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
        if(!GoldData) return;
		switch (It.Key)
		{
			break;
			case 1: // MaxHP 증가 , 합연산
			HPPlus = HPPlus + GoldData->Coefficient_X * It.Value;
			break;

			case 2: // MaxHP 증가 , 곱연산
			HPCoeff = HPCoeff + GoldData->Coefficient_X * It.Value;
			break;
		}
    }
	RowName = RowDataTable->PlatinumRowName;
    for(auto It : PassiveNestingData.PlatinumAbilityNestingNum)
    {
		auto PlatinumData = APGI->GetPlatinumAbilityDataTable()->FindRow<FSkillAbilityDataSheet>(FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
        if(!PlatinumData) return;
		switch (It.Key)
		{
			break;
			case 1: // MaxHP 증가 , 합연산
			HPPlus = HPPlus + PlatinumData->Coefficient_X * It.Value;
			break;

			case 2: // MaxHP 증가 , 곱연산
			HPCoeff = HPCoeff + PlatinumData->Coefficient_X * It.Value;
			break;
		}
    }

	MaxHP = MaxHP + HPPlus; 
	HPCoeff = HPCoeff - Previous_MaxHPCoeff;
	OwnerCharacter->GetBuffComp()->AddMaxHPCoefficient(HPCoeff);
	OwnerCharacter->SetDefaultMaxHP_Origin(MaxHP);
	OwnerCharacter->GetAPHUD()->OnUpdateMaxHPBar.Broadcast(MaxHP);
	Previous_MaxHPCoeff = Previous_MaxHPCoeff + HPCoeff;
}

void UAPPassiveComponent::UpdateATK()
{
	auto ATK = Status_Init.PlayerDynamicData.ATK;

	float ATKPlus = 5.0f;
	float ATKCoeff = 0.05f; 	// 기본 증가 // 하드코딩
	float MaxHPSubCoeff = 0.0f; // 최대 체력 감소 계수

	// 증강 증가
	FName PassiveName = TEXT("Passive_2");
	auto RowDataTable = APGI->GetSkillAbilityRowData()->FindRow<FSkillAbilityRowNameData>(PassiveName, PassiveName.ToString());
	auto PassiveNestingData = OwnerCharacter->GetPassiveSkills()[(uint8)EPassiveNumber::Passive_2];

	auto RowName = RowDataTable->SilverRowName;
	for(auto It : PassiveNestingData.SilverAbilityNestingNum)
    {
		auto SilverData = APGI->GetSilverAbilityDataTable()->FindRow<FSkillAbilityDataSheet>( FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
        if(!SilverData) return;
		switch (It.Key)
		{
			case 1: // ATK 증가 , 합연산
			ATKPlus = ATKPlus + SilverData->Coefficient_X * It.Value;
			break;
			case 2: // ATK 증가 , 곱연산
			ATKCoeff = ATKCoeff + SilverData->Coefficient_X * It.Value;
			break;
			case 3: // ATK 증가 , 곱연산 , MaxHP 감소
			ATKCoeff = ATKCoeff + SilverData->Coefficient_X * It.Value;
			MaxHPSubCoeff = MaxHPSubCoeff - SilverData->Coefficient_Y * It.Value;
			break;
		}
    }
	RowName = RowDataTable->GoldRowName;
	for(auto It : PassiveNestingData.GoldAbilityNestingNum)
    {
		auto GoldData = APGI->GetGoldAbilityDataTable()->FindRow<FSkillAbilityDataSheet>( FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
        if(!GoldData) return;
		switch (It.Key)
		{
			case 1: // ATK 증가 , 합연산
			ATKPlus = ATKPlus + GoldData->Coefficient_X * It.Value;
			break;
			case 2: // ATK 증가 , 곱연산
			ATKCoeff = ATKCoeff + GoldData->Coefficient_X * It.Value;
			break;
			case 3: // ATK 증가 , 곱연산 , MaxHP 감소
			ATKCoeff = ATKCoeff + GoldData->Coefficient_X * It.Value;
			MaxHPSubCoeff = MaxHPSubCoeff - GoldData->Coefficient_Y * It.Value;
			break;
		}
    }
	RowName = RowDataTable->PlatinumRowName;
	for(auto It : PassiveNestingData.PlatinumAbilityNestingNum)
    {
		auto PlatinumData = APGI->GetPlatinumAbilityDataTable()->FindRow<FSkillAbilityDataSheet>( FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
        if(!PlatinumData) return;
		switch (It.Key)
		{
			case 1: // ATK 증가 , 합연산
			ATKPlus = ATKPlus + PlatinumData->Coefficient_X * It.Value;
			break;
			case 2: // ATK 증가 , 곱연산
			ATKCoeff = ATKCoeff + PlatinumData->Coefficient_X * It.Value;
			break;
			case 3: // ATK 증가 , 곱연산 , MaxHP 감소
			ATKCoeff = ATKCoeff + PlatinumData->Coefficient_X * It.Value;
			MaxHPSubCoeff = MaxHPSubCoeff - PlatinumData->Coefficient_Y * It.Value;
			break;
		}
    }

	ATKCoeff = ATKCoeff - Previous_ATKCoeff;
	OwnerCharacter->GetBuffComp()->AddATKCoefficient(ATKCoeff);
	MaxHPSubCoeff = MaxHPSubCoeff - Previous_MaxHPSubCoeff;
	OwnerCharacter->GetBuffComp()->AddMaxHPCoefficient(MaxHPSubCoeff);
	ATK = ATK + ATKPlus;
	OwnerCharacter->SetDefaultATK_Origin(ATK);
	OwnerCharacter->GetAPHUD()->OnUpdateMaxHPBar.Broadcast(0.0f);

	Previous_ATKCoeff = Previous_ATKCoeff + ATKCoeff;
	Previous_MaxHPSubCoeff = Previous_MaxHPSubCoeff + MaxHPSubCoeff;
}

void UAPPassiveComponent::UpdateATKSpeed()
{
	auto ATKSpeed = Status_Init.PlayerDynamicData.ATKSpeed;

	float ATKSpeedCoeff = 0.07f; 	// 기본 증가 // 하드코딩
	
	// 증강 증가
	FName PassiveName = TEXT("Passive_3");
	auto RowDataTable = APGI->GetSkillAbilityRowData()->FindRow<FSkillAbilityRowNameData>(PassiveName, PassiveName.ToString());
	auto PassiveNestingData = OwnerCharacter->GetPassiveSkills()[(uint8)EPassiveNumber::Passive_3];

	auto RowName = RowDataTable->SilverRowName;
	for(auto It : PassiveNestingData.SilverAbilityNestingNum)
    {
		auto SilverData = APGI->GetSilverAbilityDataTable()->FindRow<FSkillAbilityDataSheet>( FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
		if(!SilverData) return;
		switch (It.Key)
		{
			case 1: // ATKSpeed 증가 , 곱연산
			ATKSpeedCoeff = ATKSpeedCoeff + SilverData->Coefficient_X * It.Value;
			break;
		}
    }
	RowName = RowDataTable->GoldRowName;
    for(auto It : PassiveNestingData.GoldAbilityNestingNum)
    {
		auto GoldData = APGI->GetGoldAbilityDataTable()->FindRow<FSkillAbilityDataSheet>( FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
        if(!GoldData) return;
		switch (It.Key)
		{
			case 1: // ATKSpeed 증가 , 곱연산
			ATKSpeedCoeff = ATKSpeedCoeff + GoldData->Coefficient_X * It.Value;
			break;
		}
    }
	RowName = RowDataTable->PlatinumRowName;
    for(auto It : PassiveNestingData.PlatinumAbilityNestingNum)
    {
		auto PlatinumData = APGI->GetPlatinumAbilityDataTable()->FindRow<FSkillAbilityDataSheet>(FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
        if(!PlatinumData) return;
		switch (It.Key)
		{
			case 1: // ATKSpeed 증가 , 곱연산
			ATKSpeedCoeff = ATKSpeedCoeff + PlatinumData->Coefficient_X * It.Value;
			break;
		}
    }

	ATKSpeedCoeff = ATKSpeedCoeff - Previous_ATKSpeedCoeff;
	OwnerCharacter->GetBuffComp()->AddATKSpeedCoefficient(ATKSpeedCoeff);
	OwnerCharacter->SetDefaultATKSpeed_Origin(ATKSpeed);
	Previous_ATKSpeedCoeff = Previous_ATKSpeedCoeff + ATKSpeedCoeff;
}

void UAPPassiveComponent::UpdateMoveSpeed()
{
	auto MoveSpeed = Status_Init.PlayerDynamicData.MoveSpeed;

	float MoveSpeedPlus = 0.0f;
	float MoveSpeedCoeff = 0.05f; 	// 기본 증가 // 하드코딩
	
	// 증강 증가
	FName PassiveName = TEXT("Passive_4");
	auto RowDataTable = APGI->GetSkillAbilityRowData()->FindRow<FSkillAbilityRowNameData>(PassiveName, PassiveName.ToString());
	auto PassiveNestingData = OwnerCharacter->GetPassiveSkills()[(uint8)EPassiveNumber::Passive_4];

	auto RowName = RowDataTable->SilverRowName;
	for(auto It : PassiveNestingData.SilverAbilityNestingNum)
    {
		auto SilverData = APGI->GetSilverAbilityDataTable()->FindRow<FSkillAbilityDataSheet>( FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
		if(!SilverData) return;
		switch (It.Key)
		{
			case 1: // Speed 증가 , 합연산
			MoveSpeedPlus = MoveSpeedPlus + SilverData->Coefficient_X * It.Value;
			break;
			case 2: // Speed 증가 , 곱연산
			MoveSpeedCoeff = MoveSpeedCoeff + SilverData->Coefficient_X * It.Value;
			break;
		}
    }
	RowName = RowDataTable->GoldRowName;
    for(auto It : PassiveNestingData.GoldAbilityNestingNum)
    {
		auto GoldData = APGI->GetGoldAbilityDataTable()->FindRow<FSkillAbilityDataSheet>( FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
        if(!GoldData) return;
		switch (It.Key)
		{
			case 1: // Speed 증가 , 합연산
			MoveSpeedPlus = MoveSpeedPlus + GoldData->Coefficient_X * It.Value;
			break;
			case 2: // Speed 증가 , 곱연산
			MoveSpeedCoeff = MoveSpeedCoeff + GoldData->Coefficient_X * It.Value;
			break;
		}
    }
	RowName = RowDataTable->PlatinumRowName;
    for(auto It : PassiveNestingData.PlatinumAbilityNestingNum)
    {
		auto PlatinumData = APGI->GetPlatinumAbilityDataTable()->FindRow<FSkillAbilityDataSheet>(FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
        if(!PlatinumData) return;
		switch (It.Key)
		{
			case 1: // Speed 증가 , 합연산
			MoveSpeedPlus = MoveSpeedPlus + PlatinumData->Coefficient_X * It.Value;
			break;
			case 2: // Speed 증가 , 곱연산
			MoveSpeedCoeff = MoveSpeedCoeff + PlatinumData->Coefficient_X * It.Value;
			break;
		}
    }

	MoveSpeed = MoveSpeed + MoveSpeedPlus;
	MoveSpeedCoeff = MoveSpeedCoeff - Previous_MoveSpeedCoeff;
	OwnerCharacter->GetCrowdControlComp()->AddMoveSpeedCoefficient(MoveSpeedCoeff);
	OwnerCharacter->SetDefaultSpeed_Origin(MoveSpeed);
	Previous_MoveSpeedCoeff = Previous_MoveSpeedCoeff + MoveSpeedCoeff;
}

void UAPPassiveComponent::UpdateMaxMP()
{
	auto MaxMP = OwnerCharacter->GetDefaultMaxMP();

	uint8 MPPlus = 0;
	float MPRecoveryCoeff = 0.9f; 	// 기본 증가 // 하드코딩
	
	// 증강 증가
	FName PassiveName = TEXT("Passive_5");
	auto RowDataTable = APGI->GetSkillAbilityRowData()->FindRow<FSkillAbilityRowNameData>(PassiveName, PassiveName.ToString());
	auto PassiveNestingData = OwnerCharacter->GetPassiveSkills()[(uint8)EPassiveNumber::Passive_5];

	auto RowName = RowDataTable->SilverRowName;
	for(auto It : PassiveNestingData.SilverAbilityNestingNum)
    {
		auto SilverData = APGI->GetSilverAbilityDataTable()->FindRow<FSkillAbilityDataSheet>( FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
		if(!SilverData) return;
		switch (It.Key)
		{
			case 1: // MaxHP 증가 , 합연산
			MPRecoveryCoeff = MPRecoveryCoeff - SilverData->Coefficient_X * It.Value;
			break;
		}
    }
	RowName = RowDataTable->GoldRowName;
    for(auto It : PassiveNestingData.GoldAbilityNestingNum)
    {
		auto GoldData = APGI->GetGoldAbilityDataTable()->FindRow<FSkillAbilityDataSheet>( FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
        if(!GoldData) return;
		switch (It.Key)
		{
			break;
			case 1: // MaxHP 증가 , 합연산
			MPRecoveryCoeff = MPRecoveryCoeff - GoldData->Coefficient_X * It.Value;
			break;
		}
    }
	RowName = RowDataTable->PlatinumRowName;
    for(auto It : PassiveNestingData.PlatinumAbilityNestingNum)
    {
		auto PlatinumData = APGI->GetPlatinumAbilityDataTable()->FindRow<FSkillAbilityDataSheet>(FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
        if(!PlatinumData) return;
		switch (It.Key)
		{
			break;
			case 1: // MaxHP 증가 , 합연산
			MPRecoveryCoeff = MPRecoveryCoeff - PlatinumData->Coefficient_X * It.Value;
			break;

			break;
			case 2: // MaxHP 증가 , 합연산
			MPPlus = MPPlus + PlatinumData->Coefficient_X * It.Value;
			break;
		}
    }

	MPPlus = MPPlus - Previous_MaxMPPlus;
	MaxMP = MaxMP + MPPlus; 
	OwnerCharacter->SetDefaultMaxMP_Origin(MaxMP);
	OwnerCharacter->SetDefaultMP(OwnerCharacter->GetDefaultMP() + MPPlus);
	OwnerCharacter->GetAPHUD()->OnUpdateMaxMPBar.Broadcast(MPPlus);
	OwnerCharacter->GetAPSkillHubComponent()->SetRecoveryTime_MP(MPRecoveryTime_Init * MPRecoveryCoeff);
	Previous_MaxMPPlus = Previous_MaxMPPlus + MPPlus;
}

void UAPPassiveComponent::UpdateCriticalPercent()
{
	auto CriticalPercent = Status_Init.PlayerDynamicData.CriticalPercent;
	auto CriticalDamageCoefficient = Status_Init.PlayerDynamicData.CriticalDamageCoefficient;

	float CriticalPercentPlus = 2.0f; 	// 기본 증가 // 하드코딩
	float CriticalDamagePlus = 0.0f;

	// 증강 증가
	FName PassiveName = TEXT("Passive_6");
	auto RowDataTable = APGI->GetSkillAbilityRowData()->FindRow<FSkillAbilityRowNameData>(PassiveName, PassiveName.ToString());
	auto PassiveNestingData = OwnerCharacter->GetPassiveSkills()[(uint8)EPassiveNumber::Passive_6];

	auto RowName = RowDataTable->SilverRowName;
	for(auto It : PassiveNestingData.SilverAbilityNestingNum)
    {
		auto SilverData = APGI->GetSilverAbilityDataTable()->FindRow<FSkillAbilityDataSheet>( FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
		if(!SilverData) return;
		switch (It.Key)
		{
			case 1: // CriticalPercent 증가 , 합연산
			CriticalPercentPlus = CriticalPercentPlus + SilverData->Coefficient_X * It.Value * 100.0f;
			break;
			case 2: // CriticalDamage 증가 , 합연산
			CriticalDamagePlus = CriticalDamagePlus + SilverData->Coefficient_X * It.Value;
			break;
		}
    }
	RowName = RowDataTable->GoldRowName;
    for(auto It : PassiveNestingData.GoldAbilityNestingNum)
    {
		auto GoldData = APGI->GetGoldAbilityDataTable()->FindRow<FSkillAbilityDataSheet>( FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
        if(!GoldData) return;
		switch (It.Key)
		{
			case 1: // CriticalPercent 증가 , 합연산
			CriticalPercentPlus = CriticalPercentPlus + GoldData->Coefficient_X * It.Value * 100.0f;
			break;
			case 2: // CriticalDamage 증가 , 합연산
			CriticalDamagePlus = CriticalDamagePlus + GoldData->Coefficient_X * It.Value;
			break;
		}
    }
	RowName = RowDataTable->PlatinumRowName;
    for(auto It : PassiveNestingData.PlatinumAbilityNestingNum)
    {
		auto PlatinumData = APGI->GetPlatinumAbilityDataTable()->FindRow<FSkillAbilityDataSheet>(FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
        if(!PlatinumData) return;
		switch (It.Key)
		{
			case 1: // CriticalPercent 증가 , 합연산
			CriticalPercentPlus = CriticalPercentPlus + PlatinumData->Coefficient_X * It.Value * 100.0f;
			break;
			case 2: // CriticalDamage 증가 , 합연산
			CriticalDamagePlus = CriticalDamagePlus + PlatinumData->Coefficient_X * It.Value;
			break;
		}
    }

	CriticalPercent = CriticalPercent + CriticalPercentPlus;
	OwnerCharacter->SetDefaultCriticalPercent_Origin(CriticalPercent);

	CriticalDamageCoefficient = CriticalDamageCoefficient + CriticalDamagePlus;
	OwnerCharacter->SetDefaultCriticalDamage_Origin(CriticalDamageCoefficient);
	// Previous_CriticalPercentPlus = Previous_CriticalPercentPlus + CriticalPercentPlus;
}

void UAPPassiveComponent::UpdateDEF()
{
	auto DEF = Status_Init.PlayerDynamicData.DEF;

	float DEFPlus = 5.0f; // 기본 증가 // 하드코딩
	float DEFCoeff = 0.0f;

	// 증강 증가
	FName PassiveName = TEXT("Passive_7");
	auto RowDataTable = APGI->GetSkillAbilityRowData()->FindRow<FSkillAbilityRowNameData>(PassiveName, PassiveName.ToString());
	auto PassiveNestingData = OwnerCharacter->GetPassiveSkills()[(uint8)EPassiveNumber::Passive_7];

	auto RowName = RowDataTable->SilverRowName;
	for(auto It : PassiveNestingData.SilverAbilityNestingNum)
    {
		auto SilverData = APGI->GetSilverAbilityDataTable()->FindRow<FSkillAbilityDataSheet>( FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
		if(!SilverData) return;
		switch (It.Key)
		{
			case 1: // DEF 증가 , 합연산
			DEFPlus = DEFPlus + SilverData->Coefficient_X * It.Value;
			break;
			case 2: // DEF 증가 , 곱연산
			DEFCoeff = DEFCoeff + SilverData->Coefficient_X * It.Value;
			break;
		}
    }
	RowName = RowDataTable->GoldRowName;
    for(auto It : PassiveNestingData.GoldAbilityNestingNum)
    {
		auto GoldData = APGI->GetGoldAbilityDataTable()->FindRow<FSkillAbilityDataSheet>( FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
        if(!GoldData) return;
		switch (It.Key)
		{
			case 1: // DEF 증가 , 합연산
			DEFPlus = DEFPlus + GoldData->Coefficient_X * It.Value;
			break;
			case 2: // DEF 증가 , 곱연산
			DEFCoeff = DEFCoeff + GoldData->Coefficient_X * It.Value;
			break;
		}
    }
	RowName = RowDataTable->PlatinumRowName;
    for(auto It : PassiveNestingData.PlatinumAbilityNestingNum)
    {
		auto PlatinumData = APGI->GetPlatinumAbilityDataTable()->FindRow<FSkillAbilityDataSheet>(FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
        if(!PlatinumData) return;
		switch (It.Key)
		{
			case 1: // DEF 증가 , 합연산
			DEFPlus = DEFPlus + PlatinumData->Coefficient_X * It.Value;
			break;
			case 2: // DEF 증가 , 곱연산
			DEFCoeff = DEFCoeff + PlatinumData->Coefficient_X * It.Value;
			break;
		}
    }

	DEF = DEF + DEFPlus;
	DEFCoeff = DEFCoeff - Previous_DEFCoeff;
	OwnerCharacter->GetBuffComp()->AddDEFCoefficient(DEFCoeff);
	OwnerCharacter->SetDefaultDEF_Origin(DEF);
	Previous_DEFCoeff = Previous_DEFCoeff + DEFCoeff;
}

void UAPPassiveComponent::UpdateGoldPercent()
{
	auto GoldPercent = GoldPercent_Init;

	float GoldPlus = 0.05f; // 기본 증가 // 하드코딩
	float DoubleGoldPercent = 0.0f;
	HPRecoveryPercent = 0.0f;
	HPRecoveryAmount = 0.0f;

	// 증강 증가
	FName PassiveName = TEXT("Passive_8");
	auto RowDataTable = APGI->GetSkillAbilityRowData()->FindRow<FSkillAbilityRowNameData>(PassiveName, PassiveName.ToString());
	auto PassiveNestingData = OwnerCharacter->GetPassiveSkills()[(uint8)EPassiveNumber::Passive_8];

	auto RowName = RowDataTable->SilverRowName;
	for(auto It : PassiveNestingData.SilverAbilityNestingNum)
    {
		auto SilverData = APGI->GetSilverAbilityDataTable()->FindRow<FSkillAbilityDataSheet>( FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
		if(!SilverData) return;
		switch (It.Key)
		{
			case 1: // Gold 획득률 증가 , 합연산
			GoldPlus = GoldPlus + SilverData->Coefficient_X * It.Value;
			break;

			case 2: // 두배 Gold 획득률 증가 , 합연산
			DoubleGoldPercent = DoubleGoldPercent + SilverData->Coefficient_X * It.Value * 100.0f;
			break;
		}
    }
	RowName = RowDataTable->GoldRowName;
    for(auto It : PassiveNestingData.GoldAbilityNestingNum)
    {
		auto GoldData = APGI->GetGoldAbilityDataTable()->FindRow<FSkillAbilityDataSheet>( FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
        if(!GoldData) return;
		switch (It.Key)
		{
			case 1: // Gold 획득률 증가 , 합연산
			GoldPlus = GoldPlus + GoldData->Coefficient_X * It.Value;
			break;

			case 2: // 두배 Gold 획득률 증가 , 합연산
			DoubleGoldPercent = DoubleGoldPercent + GoldData->Coefficient_X * It.Value * 100.0f;
			break;

			case 3: // 골드 획득 시 체력 회복
			MoneyRecovery = true;
			HPRecoveryPercent = HPRecoveryPercent + GoldData->Coefficient_X * It.Value * 100.0f;
			HPRecoveryAmount = GoldData->Coefficient_Y; // 중첩 X?
			break;
		}
    }
	RowName = RowDataTable->PlatinumRowName;
    for(auto It : PassiveNestingData.PlatinumAbilityNestingNum)
    {
		auto PlatinumData = APGI->GetPlatinumAbilityDataTable()->FindRow<FSkillAbilityDataSheet>(FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
        if(!PlatinumData) return;
		switch (It.Key)
		{
			case 1: // Gold 획득률 증가 , 합연산
			GoldPlus = GoldPlus + PlatinumData->Coefficient_X * It.Value;
			break;

			case 2: // 두배 Gold 획득률 증가 , 합연산
			DoubleGoldPercent = DoubleGoldPercent + PlatinumData->Coefficient_X * It.Value * 100.0f;
			break;

			case 3: // 머니 파워
			if(MoneyPower) break;
			MoneyPower = true;
			MoneyPowerPercent = PlatinumData->Coefficient_X * It.Value ;
			if(OwnerCharacter->GetInventory()->FindItembyId(TEXT("Gold")))
			{
				ApplyMoneyPower(OwnerCharacter->GetInventory()->FindItembyId(TEXT("Gold"))->Quantity);
			}
			break;
		}
    }

	GoldPercent = GoldPercent + GoldPlus;
	if(!APGI.IsValid()) return;
	APGI->GetPackageDropMap()[TEXT_Gold] = GoldPercent * 100.0f;
	APGI->SetGoldPlusPercent(DoubleGoldPercent);
}

void UAPPassiveComponent::UpdateDamaged()
{
	DamagedPassive = true;

	// 증강 증가
	FName PassiveName = TEXT("Passive_9");
	auto RowDataTable = APGI->GetSkillAbilityRowData()->FindRow<FSkillAbilityRowNameData>(PassiveName, PassiveName.ToString());
	auto PassiveNestingData = OwnerCharacter->GetPassiveSkills()[(uint8)EPassiveNumber::Passive_9];

	auto RowName = RowDataTable->SilverRowName;

	DamagedGoldPercent = 0.0f;
	for(auto It : PassiveNestingData.SilverAbilityNestingNum)
    {
		auto SilverData = APGI->GetSilverAbilityDataTable()->FindRow<FSkillAbilityDataSheet>( FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
		if(!SilverData) return;
		switch (It.Key)
		{
			case 1: // Gold 획득률 증가 , 합연산
			DamagedGoldPercent = DamagedGoldPercent + SilverData->Coefficient_X * It.Value * 100.0f;
			break;
		}
    }
	RowName = RowDataTable->GoldRowName;
    for(auto It : PassiveNestingData.GoldAbilityNestingNum)
    {
		auto GoldData = APGI->GetGoldAbilityDataTable()->FindRow<FSkillAbilityDataSheet>( FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
        if(!GoldData) return;
		switch (It.Key)
		{
			case 1: // Gold 획득률 증가 , 합연산
			DamagedGoldPercent = DamagedGoldPercent + GoldData->Coefficient_X * It.Value * 100.0f;
			break;
		}
    }
	RowName = RowDataTable->PlatinumRowName;
    for(auto It : PassiveNestingData.PlatinumAbilityNestingNum)
    {
		auto PlatinumData = APGI->GetPlatinumAbilityDataTable()->FindRow<FSkillAbilityDataSheet>(FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
        if(!PlatinumData) return;
		switch (It.Key)
		{
			case 1: // Gold 획득률 증가 , 합연산
			DamagedGoldPercent = DamagedGoldPercent + PlatinumData->Coefficient_X * It.Value * 100.0f;
			break;
		}
    }

}

void UAPPassiveComponent::UpdateDrain()
{
	FName PassiveName = TEXT("Passive_10");
	auto RowDataTable = APGI->GetSkillAbilityRowData()->FindRow<FSkillAbilityRowNameData>(PassiveName, PassiveName.ToString());
	auto PassiveNestingData = OwnerCharacter->GetPassiveSkills()[(uint8)EPassiveNumber::Passive_10];

	
	float DrainPercent = 0.05f;

	auto RowName = RowDataTable->SilverRowName;
	for(auto It : PassiveNestingData.SilverAbilityNestingNum)
    {
		auto SilverData = APGI->GetSilverAbilityDataTable()->FindRow<FSkillAbilityDataSheet>( FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
		if(!SilverData) return;
		switch (It.Key)
		{
			case 1: // DrainPercent 증가 , 합연산
			DrainPercent = DrainPercent + SilverData->Coefficient_X * It.Value;
			break;
		}
    }
	RowName = RowDataTable->GoldRowName;
    for(auto It : PassiveNestingData.GoldAbilityNestingNum)
    {
		auto GoldData = APGI->GetGoldAbilityDataTable()->FindRow<FSkillAbilityDataSheet>( FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
        if(!GoldData) return;
		switch (It.Key)
		{
			case 1: // DrainPercent 증가 , 합연산
			DrainPercent = DrainPercent + GoldData->Coefficient_X * It.Value;
			break;
		}
    }
	RowName = RowDataTable->PlatinumRowName;
    for(auto It : PassiveNestingData.PlatinumAbilityNestingNum)
    {
		auto PlatinumData = APGI->GetPlatinumAbilityDataTable()->FindRow<FSkillAbilityDataSheet>(FName(*RowName[It.Key - 1]), RowName[It.Key - 1]);
        if(!PlatinumData) return;
		switch (It.Key)
		{
			case 1: // DrainPercent 증가 , 합연산
			DrainPercent = DrainPercent + PlatinumData->Coefficient_X * It.Value;
			break;
		}
    }

	DrainPercent = DrainPercent - Previous_DrainCoeff;
	OwnerCharacter->GetAttackComponent()->AddDrainCoefficient(DrainPercent);	// 평타만
	Previous_DrainCoeff = Previous_DrainCoeff + DrainPercent;
}
