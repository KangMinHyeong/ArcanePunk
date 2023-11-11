// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "APInventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated);

class UAPItemBase;

UENUM(BlueprintType)
enum class EItemAddResult : uint8
{
	IAR_NoItemAdded			UMETA(DisplayName = "No item added"),
	IAR_PartialAmountAdded	UMETA(DisplayName = "Partial amount of item added"),
	IAR_AllItemAdded		UMETA(DisplayName = "All of item added")
};

USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_BODY()

	FItemAddResult() : ActualAmountAdded(0), OperationResult(EItemAddResult::IAR_NoItemAdded), ResultMessage(FText::GetEmpty()) {};

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	int32 ActualAmountAdded;

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	EItemAddResult OperationResult;

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	FText ResultMessage;


	static FItemAddResult AddedNone(const FText& ErrorText)
	{
		FItemAddResult AddedNoneResult;
		AddedNoneResult.ActualAmountAdded = 0;
		AddedNoneResult.OperationResult = EItemAddResult::IAR_NoItemAdded;
		AddedNoneResult.ResultMessage = ErrorText;
		return AddedNoneResult;
	};

	static FItemAddResult AddedPartial(const int32 PartialAmountAdded, const FText& ErrorText)
	{
		FItemAddResult AddedPartialResult;
		AddedPartialResult.ActualAmountAdded = PartialAmountAdded;
		AddedPartialResult.OperationResult = EItemAddResult::IAR_PartialAmountAdded;
		AddedPartialResult.ResultMessage = ErrorText;
		return AddedPartialResult;
	};

	static FItemAddResult AddedAll(const int32 AmountAdded, const FText& Message)
	{
		FItemAddResult AddedAllResult;
		AddedAllResult.ActualAmountAdded = 0;
		AddedAllResult.OperationResult = EItemAddResult::IAR_AllItemAdded;
		AddedAllResult.ResultMessage = Message;
		return AddedAllResult;
	};
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	FOnInventoryUpdated OnInventoryUpdated;

	// Sets default values for this component's properties
	UAPInventoryComponent();

	UFUNCTION(Category = "Inventory")
	FItemAddResult HandleAddItem(UAPItemBase* InputItem);

	UFUNCTION(Category = "Inventory")
	UAPItemBase* FindMatchingItem(UAPItemBase* ItemIn) const;
	UFUNCTION(Category = "Inventory")
	UAPItemBase* FindNextItemByID(UAPItemBase* ItemIn) const;
	UFUNCTION(Category = "Inventory")
	UAPItemBase* FindNextPartialStack(UAPItemBase* ItemIn) const;

	UFUNCTION(Category = "Inventory")
	void RemoveSingleInstaceOfItem(UAPItemBase* ItemToRemove);
	UFUNCTION(Category = "Inventory")
	int32 RemoveAmountOfItem(UAPItemBase* ItemToRemove, int32 DesiredAmountToRemove);
	UFUNCTION(Category = "Inventory")
	void SplitExistingStack(UAPItemBase* ItemIn, const int32 AmountToSplit);


	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetInventoryTotalWeight() const { return InventoryTotalWeight; };
	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetWeightCapacity() const { return InventoryWeightCapacity; };
	UFUNCTION(Category = "Inventory")
	FORCEINLINE int32 GetSlotCapacity() const { return InventorySlotsCapacity; };
	UFUNCTION(Category = "Inventory")
	FORCEINLINE TArray<UAPItemBase*> GetInventoryContents() const { return InventoryContents; };

	UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetSlotsCapacity(const int32 NewSlotsCapacity) { InventorySlotsCapacity = NewSlotsCapacity; };
	UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetWeightCapacity(const float NewWeightCapacity) { InventoryWeightCapacity = NewWeightCapacity; };


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	float InventoryTotalWeight;
	UPROPERTY(EditInstanceOnly, Category = "Inventory")
	int32 InventorySlotsCapacity;
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	float InventoryWeightCapacity;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<UAPItemBase>> InventoryContents;

	FItemAddResult HandleNonStackableItems(UAPItemBase* InputItem);

	int32 HandleStackableItems(UAPItemBase* InputItem, int32 RequestedAddAmount);
	int32 CalculateWeightAddAmount(UAPItemBase* ItemIn, int32 RequestedAddAmount);
	int32 CalculateNumberForFullStack(UAPItemBase* StackableItem, int32 InitialRequestedAddAmount);

	void AddNewItem(UAPItemBase* Item, const int32 AmountToAdd);

};
