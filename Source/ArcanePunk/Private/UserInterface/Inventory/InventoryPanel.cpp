// Fill out your copyright notice in the Description page of Project Settings.

#include "UserInterface/Inventory/InventoryPanel.h"
#include "ArcanePunk/Public/Character/ArcanePunkCharacter.h"
#include "UserInterface/Inventory/InventoryItemSlot.h"
#include "Components/Character/APInventoryComponent.h"
#include "Components/Button.h"
#include "Items/APItemBase.h"
#include "UserInterface/Inventory/ItemDragDropOperation.h"


void UInventoryPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn());
	if (PlayerCharacter)
	{
		InventoryReference = PlayerCharacter->GetInventory();
		if (InventoryReference)
		{
			InventoryReference->OnInventoryUpdated.AddUObject(this, &UInventoryPanel::RefreshInventory);
			SetInfoText();
		}
	}

	if (SortingButton)
	{
		SortingButton->OnClicked.AddDynamic(this, &UInventoryPanel::InventorySorting);
	}
}

void UInventoryPanel::RefreshInventory()
{
	if (InventoryReference && InventorySlotClass)
	{
		InventoryWrapBox->ClearChildren();
		
		//for (UAPItemBase* const& InventoryItem : InventoryReference->GetInventoryContents())
		for(auto InventoryItem : InventoryReference->GetInventoryContents())
		{
			UInventoryItemSlot* ItemSlot = CreateWidget<UInventoryItemSlot>(this, InventorySlotClass);
			ItemSlot->SetItemReference(InventoryItem);
			
			InventoryWrapBox->AddChildToWrapBox(ItemSlot);
		}

		SetInfoText();
	}
}

void UInventoryPanel::SetInfoText() const
{
	const FString WeightInfoValue{ FString::SanitizeFloat(InventoryReference->GetInventoryTotalWeight()) + " / " + FString::SanitizeFloat(InventoryReference->GetWeightCapacity()) };
	const FString CapacityInfoValue{ FString::FromInt(InventoryReference->GetInventoryContents().Num()) + " / " + FString::FromInt(InventoryReference->GetSlotCapacity()) };

	WeightInfo->SetText(FText::FromString(""));//FText::FromString(WeightInfoValue));
	CapacityInfo->SetText(FText::FromString(""));//FText::FromString(CapacityInfoValue));
}

bool UInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);

	if (ItemDragDrop->SourceItem && InventoryReference)
	{
		UAPItemBase* DraggedItem = ItemDragDrop->SourceItem;

		int32 SourceIndex = InventoryReference->GetIndexOfItem(DraggedItem);

		int32 DestinationIndex = InventoryReference->GetIndexOfHoveredItem();

		if (InventoryReference->MoveItemBetweenPanels(SourceIndex, DestinationIndex))
		{
			InventoryReference->RefreshInventory();
		}

		return true;
	}
	return false;
}

void UInventoryPanel::InventorySorting()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->InventorySort();
	}
}