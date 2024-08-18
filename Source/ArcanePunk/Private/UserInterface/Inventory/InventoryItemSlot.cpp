
#include "UserInterface/Inventory/InventoryItemSlot.h"
#include "Items/APItemBase.h"
#include "UserInterface/Inventory/DragItemVisual.h"
#include "UserInterface/Inventory/InventoryTooltip.h"
#include "UserInterface/Inventory/ItemDragDropOperation.h"

// Minhyeong
#include "ArcanePunk/Public/Character/ArcanePunkCharacter.h"
#include "Components/Character/APInventoryComponent.h"
#include "PlayerController/ArcanePunkPlayerController.h"
#include "GameFramework/Controller.h"
#include "UserInterface/HUD/Status/APStatusUI.h"

void UInventoryItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	/*
	if (ToolTipClass)
	{
		UInventoryTooltip* ToolTip = CreateWidget<UInventoryTooltip>(this, ToolTipClass);
		ToolTip->InventorySlotBeingHovered = this;
		SetToolTip(ToolTip);
	}
	*/

	PlayerCharacter = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn());
	if (PlayerCharacter)
	{
		InventoryReference = PlayerCharacter->GetInventory();
	}
}

void UInventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	
	if (ItemReference)
	{
		/*
		switch(ItemReference->ItemQuality)
		{
		case EItemQuality::Common:
			ItemBorder->SetBrushColor(FLinearColor::Gray);
			break;
		case EItemQuality::Rare:
			ItemBorder->SetBrushColor(FLinearColor::Blue);
			break;
		case EItemQuality::Unique:
			ItemBorder->SetBrushColor(FLinearColor::Red);
			break;
		default: 
			break;
		}
		*/
		ItemIcon->SetBrushFromTexture(ItemReference->ItemAssetData.Icon);

		if (ItemReference->ItemNumericData.IsStackable)
		{
			ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));
		}
		else
		{
			ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	
}

FReply UInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	// when using ui
	// left mouse is attacking
	// is not conflict ?

	// if confilct -> use 2 combi of key input + mouse input

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}
	// Minhyeong
	else if(InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		if(SlotType == ESlotType::DropPackage) {DropToInventort();}
		else {SetEquipment();}
		return Reply.Handled();
	}

	return Reply.Unhandled();
}

void UInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

}

void UInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (DragItemVisualClass)
	{
		const TObjectPtr<UDragItemVisual> DragVisual = CreateWidget<UDragItemVisual>(this, DragItemVisualClass);
		DragVisual->ItemIcon->SetBrushFromTexture(ItemReference->ItemAssetData.Icon);
		//DragVisual->ItemBorder->SetBrushColor(ItemBorder->GetBrushColor());

		ItemReference->ItemNumericData.IsStackable ? DragVisual->ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity)) : DragVisual->ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);

		UItemDragDropOperation* DragItemOperation = NewObject<UItemDragDropOperation>();
		DragItemOperation->SourceItem = ItemReference;
		DragItemOperation->SourceInventory = ItemReference->OwningInventory;

		DragItemOperation->DefaultDragVisual = DragVisual;
		DragItemOperation->Pivot = EDragPivot::TopLeft;

		OutOperation = DragItemOperation;
	}
}

bool UInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Warning, TEXT("Mouse Enter"));

	if (InventoryReference)
	{
		if (ItemReference) InventoryReference->SetIndexOfHoveredItem(ItemReference);
	}

	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

//Minhyeong
void UInventoryItemSlot::SetEquipment()
{
	if(ItemReference->ItemType == EItemType::Equipment)
	{
		switch(ItemReference->ItemStatistics.EquipType)
		{
			case EEquipType::No_Equip:
			break;
			case EEquipType::Weapon:
			ChangeEquip(1);
			break;
		}
	}
}

void UInventoryItemSlot::ChangeEquip(uint8 NewValue) // 후에 플레이어 스탯 업데이트 하기
{
	auto Character = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn()); if(!Character) return;
	auto EquipData = Character->GetEquipData(NewValue);

	switch (SlotType)
	{
		case ESlotType::Inventory: // 인벤토리 장비창에서 우클릭
		Character->SetEquipData(NewValue, ItemReference); 
		Character->GetPlayerEquipment(NewValue)->SetSkeletalMesh(ItemReference->ItemAssetData.SkelMesh);
		Character->GetInventory()->RemoveAmountOfItem(ItemReference, ItemReference->Quantity);

		UpdateEquipInventory(EquipData);
		break;

		case ESlotType::Status: // 스테이터스 장비창에서 우클릭
		Character->SetEquipData(NewValue, nullptr);
		Character->GetPlayerEquipment(NewValue)->SetSkeletalMesh(nullptr);
		UpdateEquipInventory(ItemReference);
		break;
	}
}

void UInventoryItemSlot::UpdateEquipInventory(UAPItemBase *NewData)
{
	auto Character = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn());
	if(!Character) return;

	if(NewData)
	{
		ItemReference = NewData;
		Character->GetInventory()->HandleAddItem(ItemReference);
	}
	// else return;

	// 장비 슬롯 업데이트
	auto PC = Cast<AArcanePunkPlayerController>(Character->GetController());
	if(!PC) return;

	PC->GetStatusUI()->InitEquipSlot();

	// ItemReference를 다시 EquipData로 치환해서 Slot으로 띄우기
}

void UInventoryItemSlot::DropToInventort()
{
	auto Character = Cast<AArcanePunkCharacter>(GetOwningPlayerPawn()); if(!Character) return;

	Character->GetInventory()->HandleAddItem(ItemReference);
	ItemReference = nullptr;
	RemoveFromParent();
}
