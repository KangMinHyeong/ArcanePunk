// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Drop/Enemy_DropEquipMent.h"

#include "ArcanePunk/Public/Character/ArcanePunkCharacter.h"
#include "ArcanePunk/Public/Components/APInventoryComponent.h"
#include "Items/APItemBase.h"
#include "GameFramework/ProjectileMovementComponent.h"

AEnemy_DropEquipMent::AEnemy_DropEquipMent()
{
    DropMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ImpactMovementComponent"));
	DropMovement->MaxSpeed = DropSpeed;
	DropMovement->InitialSpeed = DropSpeed;
}

void AEnemy_DropEquipMent::BeginPlay()
{
	Super::BeginPlay();

	InitializePickup(UAPItemBase::StaticClass(), ItemQuantity);
}

void AEnemy_DropEquipMent::InitializePickup(const TSubclassOf<UAPItemBase> BaseClass, const int32 Quantity)
{
	if (ItemDataTable && !DesiredItemID.IsNone())
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());

		ItemReference = NewObject<UAPItemBase>(this, BaseClass);

		// Itema Data Register

		ItemReference->ID = ItemData->ID;
		ItemReference->ItemType = ItemData->ItemType;
		ItemReference->ItemQuality = ItemData->ItemQuality;
		ItemReference->ItemNumericData = ItemData->ItemNumericData;
		ItemReference->ItemTextData = ItemData->ItemTextData;
		ItemReference->ItemAssetData = ItemData->ItemAssetData;
		ItemReference->ItemStatistics = ItemData->ItemStatistics;

		DropMesh->SetStaticMesh(ItemData->ItemAssetData.Mesh);

		Quantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(Quantity);
	}
}

void AEnemy_DropEquipMent::DropOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    auto Character = Cast<AArcanePunkCharacter>(OtherActor);
    if(!Character || IsPendingKillPending() || !ItemReference) return;

    if (UAPInventoryComponent* PlayerInventory = Character->GetInventory())
	{
		const FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);
	    switch(AddResult.OperationResult)
		{
		case EItemAddResult::IAR_NoItemAdded:
			UE_LOG(LogTemp, Warning, TEXT("No add item"));
			break;
		case EItemAddResult::IAR_PartialAmountAdded:
			Character->UpdateInteractionWidget();
			UE_LOG(LogTemp, Warning, TEXT("Yes Paritial"));
			break;
		case EItemAddResult::IAR_AllItemAdded:
			UE_LOG(LogTemp, Warning, TEXT("YES WE GET ALL ITEM"));
			Destroy();
			break;
		}

		UE_LOG(LogTemp, Warning, TEXT("%s"), *AddResult.ResultMessage.ToString());
	}
}