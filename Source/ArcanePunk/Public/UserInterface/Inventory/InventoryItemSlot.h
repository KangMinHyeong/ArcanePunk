// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InventoryItemSlot.generated.h"

class AArcanePunkCharacter;
class UAPInventoryComponent;
class UInventoryTooltip;
class UDragItemVisual;
class UAPItemBase;

UENUM()
enum class ESlotType : uint8
{
	Inventory		= 0,
    Status      	= 1,
    DropPackage 	= 2,
};


UCLASS()
class ARCANEPUNK_API UInventoryItemSlot : public UUserWidget
{
	GENERATED_BODY()


public:

	FORCEINLINE void SetItemReference(UAPItemBase* ItemIn) { ItemReference = ItemIn; };
	FORCEINLINE UAPItemBase* GetItemReference() const { return ItemReference; };


protected:

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<UDragItemVisual> DragItemVisualClass;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<UInventoryTooltip> ToolTipClass;

	UPROPERTY()
	AArcanePunkCharacter* PlayerCharacter;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot")
	UAPItemBase* ItemReference;

	UPROPERTY()
	UAPInventoryComponent* InventoryReference;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	UBorder* ItemBorder;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	UImage* ItemIcon;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	UTextBlock* ItemQuantity;


	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	//Minhyeong
	void SetEquipment();
	void ChangeEquip(uint8 NewValue);
	void UpdateEquipInventory(UAPItemBase* NewData);
	void DropToInventort();

	UPROPERTY()
	ESlotType SlotType = ESlotType::Inventory;

public:
	FORCEINLINE void SetInInventory(ESlotType NewType) { SlotType = NewType; };
};
