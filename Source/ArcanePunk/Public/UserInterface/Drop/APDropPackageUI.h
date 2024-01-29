// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserInterface/Inventory/InventoryItemSlot.h"
#include "Items/APItemBase.h"
#include "Blueprint/UserWidget.h"
#include "APDropPackageUI.generated.h"

class UWrapBox;
class UAPSlotSwitcher;
class UButton;

UCLASS()
class ARCANEPUNK_API UAPDropPackageUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;	
	virtual FReply NativeOnKeyDown( const FGeometry& InGeometry, const FKeyEvent& InKeyEvent ) override;
	FORCEINLINE void InitPackage(TArray<UAPItemBase*> NewItemsInPackage) {ItemsInPackage = NewItemsInPackage;};

private:
	void AddWidget();
	void AddPackage();
	void BindButton();

	UFUNCTION()
	void ExitPackage();
	UFUNCTION()
	void AddAllItem();
	
private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UWrapBox* TopWrapBox;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UWrapBox* BottomWrapBox;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAPSlotSwitcher> DropSlotSwitcher;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventoryItemSlot> InventorySlotClass;

	TArray<UAPSlotSwitcher*> DropSlots; 

	UPROPERTY(EditAnywhere)
	uint8 SlotNum = 16;

	UPROPERTY()
	TArray<UAPItemBase*> ItemsInPackage;

	TArray<UInventoryItemSlot*> SlotArr;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UButton* Exit_Button;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UButton* AddAll_Button;

};
