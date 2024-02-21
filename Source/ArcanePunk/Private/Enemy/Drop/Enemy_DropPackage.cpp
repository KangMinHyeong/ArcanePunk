
#include "Enemy/Drop/Enemy_DropPackage.h"

#include "Character/ArcanePunkCharacter.h"
#include "UserInterface/Drop/APDropPackageUI.h"
#include "PlayerController/ArcanePunkPlayerController.h"

AEnemy_DropPackage::AEnemy_DropPackage()
{
    IsInit = false;
	EnHanceTypePercent.SetNum(3);
}

void AEnemy_DropPackage::DropOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if(!IsInit) return;
	auto Character = Cast<AArcanePunkCharacter>(OtherActor); if(!Character) return;
	auto PC = Cast<AArcanePunkPlayerController>(Character->GetController()); if(!PC) return; 
	PC->SetPause(true);
	
	auto PackageUI = CreateWidget<UAPDropPackageUI>(PC, PackageUIClass);
	if(PackageUI) PackageUI->InitPackage(ItemsInPackage);
	PackageUI->AddToViewport();

	if(EnhanceCategory != EEnhanceCategory::None) Character->SetSkillAbility(EnhanceCategory, EnHanceType);

	Destroy();
}

void AEnemy_DropPackage::AddItem(FName ItemID)
{
    if(!ItemDataTable) return;

	const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(ItemID, ItemID.ToString());

	auto AddItemReference = NewObject<UAPItemBase>(this, UAPItemBase::StaticClass());
	AddItemReference->ID = ItemData->ID;
	AddItemReference->ItemType = ItemData->ItemType;
	AddItemReference->ItemQuality = ItemData->ItemQuality;
	AddItemReference->ItemNumericData = ItemData->ItemNumericData;
	AddItemReference->ItemTextData = ItemData->ItemTextData;
	AddItemReference->ItemAssetData = ItemData->ItemAssetData;
	AddItemReference->ItemStatistics = ItemData->ItemStatistics;
	AddItemReference->SetQuantity(1);	

    ItemsInPackage.Add(AddItemReference);
}

void AEnemy_DropPackage::AddEnhance(uint8 EnhanceCategoryNum)
{
	EnhanceCategory = static_cast<EEnhanceCategory>(EnhanceCategoryNum);

    float SilverPercent = EnHanceTypePercent[0];
    float GoldPercent = EnHanceTypePercent[0] + EnHanceTypePercent[1];
    float PlatinumPercent = EnHanceTypePercent[0] + EnHanceTypePercent[1] + EnHanceTypePercent[2];

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

}
