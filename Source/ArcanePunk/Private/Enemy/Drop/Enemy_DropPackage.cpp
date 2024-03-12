
#include "Enemy/Drop/Enemy_DropPackage.h"

#include "Character/ArcanePunkCharacter.h"
#include "UserInterface/Drop/APDropPackageUI.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Components/SphereComponent.h"

AEnemy_DropPackage::AEnemy_DropPackage()
{
    IsInit = false;
	EnHanceTypePercent.SetNum(3);
}

void AEnemy_DropPackage::BeginPlay()
{
	Super::BeginPlay();

}

void AEnemy_DropPackage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
		DropMesh->SetCustomDepthStencilValue(80);
		// To Do : 실버 이펙트 및 사운드
    }
    else if(CurrentEnHanceType > SilverPercent && CurrentEnHanceType <= GoldPercent)
    {
        EnHanceType = EEnHanceType::Gold;
		DropMesh->SetCustomDepthStencilValue(160);
		// To Do : 골드 이펙트 및 사운드
    }
    else if (CurrentEnHanceType > GoldPercent && CurrentEnHanceType <= PlatinumPercent)
    {
        EnHanceType = EEnHanceType::Platinum;
		DropMesh->SetCustomDepthStencilValue(240);
		// To Do : 플레 이펙트 및 사운드
    }
	DropMesh->SetRenderCustomDepth(true);
	
}

void AEnemy_DropPackage::BeginFocus()
{
	TWeakObjectPtr<AArcanePunkCharacter> Character = InteractTrigger->Character; if(!Character.IsValid()) return;
	Character->ActivateInteractionSweep();

	GetWorld()->GetTimerManager().SetTimer(InteractTimerHandle, this, &AEnemy_DropPackage::BeginFocus, InteractFrequency, true);
}

void AEnemy_DropPackage::EndFocus()
{
	GetWorld()->GetTimerManager().ClearTimer(InteractTimerHandle);
}

FInteractData AEnemy_DropPackage::GetInteractData()
{
    return InteractTrigger->GetInteractionData();
}

void AEnemy_DropPackage::Interact(AArcanePunkCharacter *PlayerCharacter)
{
	if(!IsInit) return; if(!PlayerCharacter) return;

	for(auto DropItems : ItemsInPackage)
	{
		PlayerCharacter->GetInventory()->HandleAddItem(DropItems);
		PlayerCharacter->InteractionActorRemove(this);
	}

	if(EnhanceCategory != EEnhanceCategory::None) PlayerCharacter->SetSkillAbility(EnhanceCategory, EnHanceType);

	auto PC = Cast<AArcanePunkPlayerController>(PlayerCharacter->GetController()); 
	if(PC) PC->CloseInteraction(this);
	Destroy();
}
