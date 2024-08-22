
#include "Enemy/Drop/Enemy_DropPackage.h"

#include "Character/ArcanePunkCharacter.h"
#include "UserInterface/Drop/APDropPackageUI.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "Components/SphereComponent.h"
#include "GameInstance/APGameInstance.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

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

    int32 Amount = 1;
	AddItemReference->SetQuantity(Amount);	

    ItemsInPackage.Add(AddItemReference);
}

int32 AEnemy_DropPackage::CheckGoldAmount()
{
    auto APGI = Cast<UAPGameInstance>(GetGameInstance()); if(!APGI) return 1;
    return APGI->CheckGoldAmount();
}

void AEnemy_DropPackage::AddEnhance()
{
	EnhanceCategory = GetRandCategory();
	if(EnhanceCategory == EEnhanceCategory::None) return;
	
    float SilverPercent = EnHanceTypePercent[0];
    float GoldPercent = EnHanceTypePercent[0] + EnHanceTypePercent[1];
    float PlatinumPercent = EnHanceTypePercent[0] + EnHanceTypePercent[1] + EnHanceTypePercent[2];

    float CurrentEnHanceType = FMath::RandRange(0.0f, PlatinumPercent);

    if(DropTrailEffect) DropTrailEffect->Activate();
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

EEnhanceCategory AEnemy_DropPackage::GetRandCategory()
{
	EEnhanceCategory Category = EEnhanceCategory::None;

    float Passive_Percent = EnHanceCategoryPercent[EEnhanceCategory::Enhance_Passive];
    float Q_Percent = Passive_Percent + EnHanceCategoryPercent[EEnhanceCategory::Enhance_Q];
    float E_Percent = Q_Percent + EnHanceCategoryPercent[EEnhanceCategory::Enhance_E];
    float R_Percent = E_Percent + EnHanceCategoryPercent[EEnhanceCategory::Enhance_R];
    
    if(R_Percent <= KINDA_SMALL_NUMBER) return EEnhanceCategory::None;

    float CurrentPercent = FMath::RandRange(0.0f, R_Percent);
    
    if(CurrentPercent >= 0.0f && CurrentPercent <= Passive_Percent)
    {
        Category = EEnhanceCategory::Enhance_Passive;
    }
    else if(CurrentPercent > Passive_Percent && CurrentPercent <= Q_Percent)
    {
        Category = EEnhanceCategory::Enhance_Q;
    }
    else if (CurrentPercent > Q_Percent && CurrentPercent <= E_Percent)
    {
        Category = EEnhanceCategory::Enhance_E;
    }
    else
    {
        Category = EEnhanceCategory::Enhance_R;
    }

    return Category;
}

void AEnemy_DropPackage::BeginFocus()
{
	TWeakObjectPtr<AArcanePunkCharacter> Character = InteractionTrigger->Character; if(!Character.IsValid()) return;
	Character->ActivateInteractionSweep();

	GetWorld()->GetTimerManager().SetTimer(InteractTimerHandle, this, &AEnemy_DropPackage::BeginFocus, InteractFrequency, true);
}

void AEnemy_DropPackage::EndFocus()
{
	GetWorld()->GetTimerManager().ClearTimer(InteractTimerHandle);
}

FInteractData AEnemy_DropPackage::GetInteractData()
{
    return InteractionTrigger->GetInteractionData();
}

void AEnemy_DropPackage::Interact(AArcanePunkCharacter *PlayerCharacter)
{
	if(!IsInit) return; if(!PlayerCharacter) return;
    if(PlayerCharacter->GetbJogging()) {PlayerCharacter->EndJog();}

	for(auto DropItems : ItemsInPackage)
	{
        if(DropItems->ID == "Gold") {DropItems->SetQuantity(CheckGoldAmount());}
		PlayerCharacter->GetInventory()->HandleAddItem(DropItems);
		PlayerCharacter->InteractionActorRemove(this);
	}

	if(EnhanceCategory != EEnhanceCategory::None) PlayerCharacter->SetSkillAbility(EnhanceCategory, EnHanceType);

	auto PC = Cast<AArcanePunkPlayerController>(PlayerCharacter->GetController()); 
	if(PC) PC->CloseInteraction(this);
	Destroy();
}

// void AEnemy_DropPackage::OnOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
// {
//     Super::OnOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

//     UNiagaraComponent* GroundComp = nullptr;
//     switch (EnHanceType)
//     {
//     case EEnHanceType::Silver:
//         GroundComp = UNiagaraFunctionLibrary::SpawnSystemAttached(GroundEffect_Silver, DropMesh, TEXT("GroundComp"), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, true);
//         break;
    
//     case EEnHanceType::Gold:
//         GroundComp = UNiagaraFunctionLibrary::SpawnSystemAttached(GroundEffect_Gold, DropMesh, TEXT("GroundComp"), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, true);
//         break;
    
//     case EEnHanceType::Platinum:
//         GroundComp = UNiagaraFunctionLibrary::SpawnSystemAttached(GroundEffect_Platinum, DropMesh, TEXT("GroundComp"), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, true);
//         break;
//     }
// }
