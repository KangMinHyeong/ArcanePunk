// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Inventory/MainMenu.h"
#include "UserInterface/Inventory/ItemDragDropOperation.h"
#include "ArcanePunk/Public/Character/ArcanePunkCharacter.h"
#include "Items/APItemBase.h"

void UMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn());
}

bool UMainMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);

	if (PlayerCharacter && ItemDragDrop->SourceItem)
	{
		PlayerCharacter->DropItems(ItemDragDrop->SourceItem, ItemDragDrop->SourceItem->Quantity);
		return true;
	}
	return false;
}
