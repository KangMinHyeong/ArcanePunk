// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/APItemBase.h"

UAPItemBase::UAPItemBase()
{
	
}

UAPItemBase* UAPItemBase::CreateItemCopy() const
{
	UAPItemBase* ItemCopy = NewObject<UAPItemBase>(StaticClass());

	ItemCopy->ID = this->ID;
	ItemCopy->Quantity = this->Quantity;
	ItemCopy->ItemQuality = this->ItemQuality;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->ItemTextData = this->ItemTextData;
	ItemCopy->ItemNumericData = this->ItemNumericData;
	ItemCopy->ItemStatistics = this->ItemStatistics;
	ItemCopy->ItemAssetData = this->ItemAssetData;

	return ItemCopy;
}

void UAPItemBase::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity, 0, ItemNumericData.bIsStackable ? ItemNumericData.MaxStackSize : 1);

		// if (OwningInventory)
		// {
		//	if (Quantity <= 0)
		//	{
		//		owningInventory->RemoveItem(this);		
		//	}
		// }
	}
}

void UAPItemBase::Use(AArcanePunkCharacter* Character)
{
	
}




