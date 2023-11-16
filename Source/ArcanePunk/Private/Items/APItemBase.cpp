// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/APItemBase.h"
#include "ArcanePunk/Public/Character/ArcanePunkCharacter.h"
#include "ArcanePunk/Public/Components/APInventoryComponent.h"

UAPItemBase::UAPItemBase() : bIsCopy(false), bIsPickup(false)
{

}

void UAPItemBase::ResetItemFlags()
{
	bIsCopy = false;
	bIsPickup = false;
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
	ItemCopy->bIsCopy = true;

	return ItemCopy;
}

void UAPItemBase::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity, 0, ItemNumericData.bIsStackable ? ItemNumericData.MaxStackSize : 1);

		if (this->OwningInventory)
		{
			if (this->Quantity <= 0)
			{
				this->OwningInventory->RemoveSingleInstaceOfItem(this);		
			}
		}
	}
}

void UAPItemBase::Use(AArcanePunkCharacter* Character)
{
	
}




