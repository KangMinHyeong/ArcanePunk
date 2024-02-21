// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WrapBox.h"
#include "ReActiveInventoryPanel.generated.h"


class UAPInventoryComponent;
class UInventoryItemSlot;
class AArcanePunkCharacter;

UCLASS()
class ARCANEPUNK_API UReActiveInventoryPanel : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
		void RefreshInventory();

	UPROPERTY(meta = (BindWidget))
		UWrapBox* InventoryWrapBox;

	UPROPERTY()
		AArcanePunkCharacter* PlayerCharacter;

	UPROPERTY()
		UAPInventoryComponent* InventoryReference;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UInventoryItemSlot> InventorySlotClass;

protected:

	virtual	void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
};
