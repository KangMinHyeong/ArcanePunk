// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/Pickup.h"
#include "ArcanePunk/Public/Character/ArcanePunkCharacter.h"
#include "ArcanePunk/Public/Components/Character/APInventoryComponent.h"
#include "Items/APItemBase.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	PickupMesh->SetSimulatePhysics(true);
	SetRootComponent(PickupMesh);
}

void APickup::BeginPlay()
{
	Super::BeginPlay();

	InitializePickup(UAPItemBase::StaticClass(), ItemQuantity);
}

void APickup::InitializePickup(const TSubclassOf<UAPItemBase> BaseClass, const int32 Quantity)
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
		
		Quantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(Quantity);

		PickupMesh->SetStaticMesh(ItemData->ItemAssetData.Mesh);

		UpdateInteractableData();
	}
}

void APickup::InitializeDrop(UAPItemBase* ItemToDrop, const int32 Quantity)
{
	ItemReference = ItemToDrop;
	Quantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(Quantity);
	ItemReference->ItemNumericData.Weight = ItemToDrop->GetItemSingleWeight();
	PickupMesh->SetStaticMesh(ItemToDrop->ItemAssetData.Mesh);

	UpdateInteractableData();
}

void APickup::UpdateInteractableData()
{
	// InstanceInteractableData.InteractableType = EInteractableType::Pickup;
	// InstanceInteractableData.Action = ItemReference->ItemTextData.InteractionText;
	// InstanceInteractableData.Name = ItemReference->ItemTextData.Name;
	// InstanceInteractableData.Quantity = ItemReference->Quantity;
	// InteractableData = InstanceInteractableData;
}

void APickup::BeginFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(true);
	}
}

void APickup::EndFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(false);
	}
}

void APickup::Interact(AArcanePunkCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		TakePickup(PlayerCharacter);
	}

}

void APickup::TakePickup(const AArcanePunkCharacter* PlayerCharacter)
{
	// player take pick up item
	if (!IsPendingKillPending())
	{
		if (ItemReference)
		{
			if (UAPInventoryComponent* PlayerInventory = PlayerCharacter->GetInventory())
			{
				const FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);

				switch(AddResult.OperationResult)
				{
				case EItemAddResult::IAR_NoItemAdded:
					UE_LOG(LogTemp, Warning, TEXT("No add item"));
					break;
				case EItemAddResult::IAR_PartialAmountAdded:
					UpdateInteractableData();
					// PlayerCharacter->UpdateInteractionWidget();
					UE_LOG(LogTemp, Warning, TEXT("Yes Paritial"));
					break;
				case EItemAddResult::IAR_AllItemAdded:
					UE_LOG(LogTemp, Warning, TEXT("YES WE GET ALL ITEM"));
					Destroy();
					break;
				default: 
					break;
				}

				UE_LOG(LogTemp, Warning, TEXT("%s"), *AddResult.ResultMessage.ToString());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Player inventory component is null"));
			}

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ItemReference is null"));
			
		}
	}
}

//void APickup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
// {
// 	Super::PostEditChangeProperty(PropertyChangedEvent);

// 	const FName ChangePropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

// 	// isvalid
// 	if (ChangePropertyName == GET_MEMBER_NAME_CHECKED(APickup, DesiredItemID))
// 	{
// 		if (ItemDataTable)
// 		{
// 			const FString ContextString{ DesiredItemID.ToString() };

// 			if (const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString()))
// 			{
// 				PickupMesh->SetStaticMesh(ItemData->ItemAssetData.Mesh);
// 			}
// 		}
// 	}
// }



