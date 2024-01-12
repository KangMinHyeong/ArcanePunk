// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APStatusUI.generated.h"

class UInventoryItemSlot;
class UImage;
class UOverlay;

UCLASS()
class ARCANEPUNK_API UAPStatusUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	void InitEquipSlot();
	
private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* WeaponEmpty;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UOverlay* WeaponOverlay;

	UPROPERTY()
	UInventoryItemSlot* WeaponSlot;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventoryItemSlot> InventorySlotClass;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"),  meta = (BindWidget))
	// UInventoryItemSlot* HeadSlot;
};
