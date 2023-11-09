// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArcanePunk/APItemDataStruct.h"
#include "UObject/NoExportTypes.h"
#include "APItemBase.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API UAPItemBase : public UObject
{
	GENERATED_BODY()

public:
	// valuables
	// 인벤토리에 쌓이는 양 ( 있을수도있고 없을수도 .. ? )
	UPROPERTY(VisibleAnywhere, Category = "Item Data", meta = (UIMin=1, UIMax=100))
	int32 Quantity;

	//UPROPERTY()
		//UInventoryComponent* OwningInventory;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemQuality ItemQuality;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemNumericData ItemNumericData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemAssetData ItemAssetData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemStatistics ItemStatistics;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemTextData ItemTextData;

	// functions
	UAPItemBase();

	UFUNCTION(Category = "Item")
	UAPItemBase* CreateItemCopy() const;

	UFUNCTION(Category="Item")
	FORCEINLINE float GetItemStackWeight() const { return Quantity * ItemNumericData.Weight; };

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemSingleWeight() const { return ItemNumericData.Weight; };

	UFUNCTION(Category = "Item")
	FORCEINLINE bool IsFullItemStack() const { return Quantity == ItemNumericData.MaxStackSize; };

	UFUNCTION(Category = "Item")
	void SetQuantity(const int32 NewQuantity);

	UFUNCTION(Category = "Item")
	virtual void Use(AArcanePunkCharacter* Character);


protected:
	// tarray find
	bool operator==(const FName& OtherID) const
	{
		return ID == OtherID;
	}
};
