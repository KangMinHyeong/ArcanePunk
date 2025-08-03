// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataStructs/Item/APItemDataStruct.h"
#include "UObject/NoExportTypes.h"
#include "APItemBase.generated.h"

class AArcanePunkCharacter;
class UAPInventoryComponent;

UCLASS()
class ARCANEPUNK_API UAPItemBase : public UObject
{
	GENERATED_BODY()

public:
	// valuables
	// �κ��丮�� ���̴� �� ( ���������ְ� �������� )
	UPROPERTY(VisibleAnywhere, Category = "Item")
	int32 Quantity;

	UPROPERTY()
	UAPInventoryComponent* OwningInventory;

	UPROPERTY(EditAnywhere, Category = "Item")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "Item")
	EItemQuality ItemQuality;

	UPROPERTY(EditAnywhere, Category = "Item")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = "Item")
	FItemNumericData ItemNumericData;

	UPROPERTY(EditAnywhere, Category = "Item")
	FItemAssetData ItemAssetData;

	UPROPERTY(EditAnywhere, Category = "Item")
	FItemStatistics ItemStatistics;

	UPROPERTY(EditAnywhere, Category = "Item")
	FItemTextData ItemTextData;

	bool bIsCopy;
	bool bIsPickup;

	// functions
	UAPItemBase();

	void ResetItemFlags();

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
		return this->ID == OtherID;
	}
};
