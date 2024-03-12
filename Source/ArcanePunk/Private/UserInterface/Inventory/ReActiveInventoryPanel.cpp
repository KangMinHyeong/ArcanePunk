// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Inventory/ReActiveInventoryPanel.h"
#include "ArcanePunk/Public/Character/ArcanePunkCharacter.h"
#include "UserInterface/Inventory/InventoryItemSlot.h"
#include "Components/APInventoryComponent.h"
#include "Components/Button.h"
#include "Items/APItemBase.h"
#include "UserInterface/Inventory/ItemDragDropOperation.h"

void UReActiveInventoryPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn());
	if (PlayerCharacter)
	{
		InventoryReference = PlayerCharacter->GetInventory();
		if (InventoryReference)
		{
			InventoryReference->OnInventoryUpdated.AddUObject(this, &UReActiveInventoryPanel::RefreshInventory);
		}
	}

}

void UReActiveInventoryPanel::RefreshInventory()
{

	if (InventoryReference && InventorySlotClass)
	{
		InventoryWrapBox->ClearChildren();

		//for (UAPItemBase* const& InventoryItem : InventoryReference->GetInventoryContents())
		for (auto InventoryItem : InventoryReference->GetInventoryContents())
		{
			UInventoryItemSlot* ItemSlot = CreateWidget<UInventoryItemSlot>(this, InventorySlotClass);
			ItemSlot->SetItemReference(InventoryItem);

			InventoryWrapBox->AddChildToWrapBox(ItemSlot);
		}
	}

	// player�� HUD->UpdateWidgetPosition(int32 ItemNumbers) ȣ��
	// PlayerCharacter->UpdateInventoryWidgetPosition(InventoryReference->GetInventoryContents().Num());
}

bool UReActiveInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
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