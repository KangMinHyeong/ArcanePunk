// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Inventory/InventoryTooltip.h"
#include "UserInterface/Inventory/InventoryItemSlot.h"
#include "Items/APItemBase.h"

void UInventoryTooltip::NativeConstruct()
{
	Super::NativeConstruct();

	UAPItemBase* ItemBeingHovered = InventorySlotBeingHovered->GetItemReference();
	if(!ItemBeingHovered) return;
	switch(ItemBeingHovered->ItemType)
	{
	case EItemType::Equipment:
		ItemType->SetText(FText::FromString("Equipment"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		ArmorRateing->SetVisibility(ESlateVisibility::Collapsed);
		UsageText->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Material:
		ItemType->SetText(FText::FromString("Material"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		ArmorRateing->SetVisibility(ESlateVisibility::Collapsed);
		UsageText->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Consumption:
		ItemType->SetText(FText::FromString("Consumption"));
		// is need ?
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		ArmorRateing->SetVisibility(ESlateVisibility::Collapsed);
		UsageText->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Goods:
		break;
	default: 
		break;
	}

	ItemName->SetText(ItemBeingHovered->ItemTextData.Name);
	DamageValue->SetText(FText::AsNumber(ItemBeingHovered->ItemStatistics.DamageValue));
	ArmorRateing->SetText(FText::AsNumber(ItemBeingHovered->ItemStatistics.ArmorRate));
	UsageText->SetText(ItemBeingHovered->ItemTextData.UsageText);
	DescriptionText->SetText(ItemBeingHovered->ItemTextData.Description);

	const FString WeightInfo = { "Weight : " + FString::SanitizeFloat(ItemBeingHovered->GetItemStackWeight()) };

	StackWeight->SetText(FText::FromString(WeightInfo));

	if (ItemBeingHovered->ItemNumericData.IsStackable)
	{
		const FString StackInfo = { "Max stack size : " + FString::FromInt(ItemBeingHovered->ItemNumericData.MaxStackSize) };

		MaxStackSize->SetText(FText::FromString(StackInfo));
	}
	else
	{
		MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
	}

}
