// Fill out your copyright notice in the Description page of Project Settings.
#include "Components/APInventoryComponent.h"
#include "Components/Button.h"
#include "Items/APItemBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UAPInventoryComponent::UAPInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	//Init();

	RefreshInventory();
}

void UAPInventoryComponent::Init()
{
	// 인벤토리 최대 용량
	// 빈 아이템 + 실제 아이템
	// 최대 용량이 10일 때,
	// 아이템 1개일때 빈 아이템은 9개여야함

	// 인벤토리 최대 용량만큼 검사 수행
	// 만약 아이템으로 꽉 차 있지 않다면 비어있는 아이템을 추가

	if (InventoryContents.Num() == 0 )
	{
		for (int32 i = 0; i < InventorySlotsCapacity; i++)
		{
			FString ItemName = FString::Printf(TEXT("NewItem%d"), EmptySlotNumbers++);
			UAPItemBase* NewItem = NewObject<UAPItemBase>(this, *ItemName);
			//CreateDefaultSubobject<UAPItemBase>(*ItemName);

			NewItem->ID = "NONE";

			InventoryContents.Add(NewItem);
		}
	}
	else
	{
		int32 size = InventorySlotsCapacity - InventoryContents.Num();

		for(int32 i = 0;i<InventorySlotsCapacity - InventoryContents.Num();i++)
		{
			FString ItemName = FString::Printf(TEXT("NewItem%d"), EmptySlotNumbers++);
			UAPItemBase* NewItem = NewObject<UAPItemBase>(this, *ItemName);
			//CreateDefaultSubobject<UAPItemBase>(*ItemName);

			NewItem->ID = "NONE";

			InventoryContents.Add(NewItem);
		}
	}

	RefreshInventory();
}

// Called when the game starts
void UAPInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}


UAPItemBase* UAPInventoryComponent::FindMatchingItem(UAPItemBase* ItemIn) const
{
	if (ItemIn)
	{
		if (InventoryContents.Contains(ItemIn))
		{
			return ItemIn;
		}
	}
	return nullptr;
}

UAPItemBase* UAPInventoryComponent::FindNextItemByID(UAPItemBase* ItemIn) const
{
	if (ItemIn)
	{
		if (const TArray<TObjectPtr<UAPItemBase>>::ElementType* Result = InventoryContents.FindByKey(ItemIn))
		{
			return *Result;
		}
	}
	return nullptr;
}

UAPItemBase* UAPInventoryComponent::FindNextPartialStack(UAPItemBase* ItemIn) const
{
	// find with condition
	if (const TArray<TObjectPtr<UAPItemBase>>::ElementType * Result = InventoryContents.FindByPredicate([&ItemIn](const UAPItemBase* InventoryItem)
		{
			// is in inventory ? is tack full ?
			return InventoryItem->ID == ItemIn->ID && !InventoryItem->IsFullItemStack();
		}
		))
	{
		// if is true
		return *Result;
	}
	return nullptr;
}

int32 UAPInventoryComponent::CalculateWeightAddAmount(UAPItemBase* ItemIn, int32 RequestedAddAmount)
{
	const int32 WeightMaxAddAmount = FMath::FloorToInt((GetWeightCapacity() - InventoryTotalWeight) / ItemIn->GetItemSingleWeight());
	if (WeightMaxAddAmount >= RequestedAddAmount)
	{
		return RequestedAddAmount;
	}
	return WeightMaxAddAmount;
}

int32 UAPInventoryComponent::CalculateNumberForFullStack(UAPItemBase* StackableItem, int32 InitialRequestedAddAmount)
{
	const int32 AddAmountToMakeFullStack = StackableItem->ItemNumericData.MaxStackSize - StackableItem->Quantity;

	return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack);
}

void UAPInventoryComponent::RemoveSingleInstaceOfItem(UAPItemBase* ItemToRemove)
{
	InventoryContents.RemoveSingle(ItemToRemove);
	ItemNumbers--;

	//Init();
	RefreshInventory();
}

int32 UAPInventoryComponent::RemoveAmountOfItem(UAPItemBase* ItemToRemove, int32 DesiredAmountToRemove)
{
	const int32 ActualAmountRemove = FMath::Min(DesiredAmountToRemove, ItemToRemove->Quantity);

	ItemToRemove->SetQuantity(ItemToRemove->Quantity - ActualAmountRemove);

	InventoryTotalWeight -= (ActualAmountRemove * ItemToRemove->GetItemSingleWeight());

	OnInventoryUpdated.Broadcast();

	return ActualAmountRemove;
}

void UAPInventoryComponent::SplitExistingStack(UAPItemBase* ItemIn, const int32 AmountToSplit)
{
	if (!(ItemNumbers + 1 > InventorySlotsCapacity))
	{
		RemoveAmountOfItem(ItemIn, AmountToSplit);
		AddNewItem(ItemIn, AmountToSplit);
	}
}

FItemAddResult UAPInventoryComponent::HandleNonStackableItems(UAPItemBase* InputItem)
{
	// add none
	if (FMath::IsNearlyZero(InputItem->GetItemSingleWeight()) || InputItem->GetItemSingleWeight() < 0)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to the inventory. Item has invalid weight value."), InputItem->ItemTextData.Name));
	}
	if (InventoryTotalWeight + InputItem->GetItemSingleWeight() > GetWeightCapacity())
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to the inventory. Item would overflow weight limit."), InputItem->ItemTextData.Name));
	}
	if (ItemNumbers + 1 > InventorySlotsCapacity)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to the inventory. Inventory is full."), InputItem->ItemTextData.Name));
	}


	// add all
	AddNewItem(InputItem, 1);
	return FItemAddResult::AddedAll(1, FText::Format(FText::FromString("Successfully added {0} {1} to the inventory."), 1, InputItem->ItemTextData.Name));
}

int32 UAPInventoryComponent::HandleStackableItems(UAPItemBase* ItemIn, int32 RequestedAddAmount)
{
	// invalid item
	if (RequestedAddAmount <= 0 || FMath::IsNearlyZero(ItemIn->GetItemStackWeight()))
	{
		return 0;
	}

	int32 AmountToDistribute = RequestedAddAmount;

	// check input item in inventory
	UAPItemBase* ExistingItemStack = FindNextPartialStack(ItemIn);

	// existed item to add
	// if enough to max stack then will find next exist item
	while(ExistingItemStack)
	{
		// how many of existing item need to full stack
		const int32 AmountToMakeFullStack = CalculateNumberForFullStack(ExistingItemStack, AmountToDistribute);
		// how many of amounttomakefullstack base on weight capacity
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ExistingItemStack, AmountToMakeFullStack);

		if (WeightLimitAddAmount > 0)
		{
			ExistingItemStack->SetQuantity(ExistingItemStack->Quantity + WeightLimitAddAmount);
			InventoryTotalWeight += (ExistingItemStack->GetItemSingleWeight() * WeightLimitAddAmount);

			AmountToDistribute -= WeightLimitAddAmount;

			ItemIn->SetQuantity(AmountToDistribute);

			if (InventoryTotalWeight >= InventoryWeightCapacity)
			{
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}
		}
		else if (WeightLimitAddAmount <= 0)
		{
			if (AmountToDistribute != RequestedAddAmount)
			{
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}

			return 0;
		}

		if (AmountToDistribute <= 0)
		{
			OnInventoryUpdated.Broadcast();
			return RequestedAddAmount;
		}

		ExistingItemStack = FindNextPartialStack(ItemIn);
	}

	// add new
	//if (InventoryContents.Num() + 1 <= InventorySlotsCapacity)
	if (ItemNumbers + 1 <= InventorySlotsCapacity)
	{
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ItemIn, AmountToDistribute);

		if (WeightLimitAddAmount > 0)
		{
			if (WeightLimitAddAmount < AmountToDistribute)
			{
				AmountToDistribute -= WeightLimitAddAmount;
				ItemIn->SetQuantity(AmountToDistribute);

				AddNewItem(ItemIn->CreateItemCopy(), WeightLimitAddAmount);
				return RequestedAddAmount - AmountToDistribute;
			}

			AddNewItem(ItemIn, AmountToDistribute);
			return RequestedAddAmount;
		}
	}

	RefreshInventory();
	return RequestedAddAmount - AmountToDistribute;
}

FItemAddResult UAPInventoryComponent::HandleAddItem(UAPItemBase* InputItem)
{
	if (GetOwner())
	{
		const int32 InitialRequestedAddAmount = InputItem->Quantity;

		if (!InputItem->ItemNumericData.IsStackable)
		{
			return HandleNonStackableItems(InputItem);
		}

		const int32 StackableAmountAdded = HandleStackableItems(InputItem, InitialRequestedAddAmount);

		// add all
		if (StackableAmountAdded == InitialRequestedAddAmount)
		{
			return FItemAddResult::AddedAll(InitialRequestedAddAmount, FText::Format(FText::FromString("Successfully added {0} {1} to the inventory."), InitialRequestedAddAmount, InputItem->ItemTextData.Name));
		}

		// add partial
		if (StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0)
		{
			return FItemAddResult::AddedPartial(InitialRequestedAddAmount, FText::Format(FText::FromString("Partial amount of {0} added to the inventory. number add = {1}."), InputItem->ItemTextData.Name, StackableAmountAdded));
		}

		// add none
		if (StackableAmountAdded <= 0)
		{
			return FItemAddResult::AddedNone(FText::Format(FText::FromString("Couldnt add {0} to the inventory. inventory is full or invalid item."), InputItem->ItemTextData.Name));
		}

	}
	
	(false);
	return FItemAddResult::AddedNone(FText::Format(FText::FromString("TryAddItem {0} error. GetWoner() check somehow fail."), InputItem->ItemTextData.Name));
}

void UAPInventoryComponent::AddNewItem(UAPItemBase* Item, const int32 AmountToAdd)
{
	UAPItemBase* NewItem;

	if (Item->bIsCopy || Item->bIsPickup)
	{
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	else
	{
		NewItem = Item->CreateItemCopy();
	}

	NewItem->OwningInventory = this;
	NewItem->SetQuantity(AmountToAdd);

	// actually add to inventory

	// 빈 아이템이 있다면 앞에서부터 채워야만 함

	InventoryContents.Add(NewItem);
	/*
	for (int32 i = 0; i < InventoryContents.Num(); i++)
	{
		if (InventoryContents[i]->ID == "NONE")
		{
			InventoryContents[i] = NewItem;
			ItemNumbers++;
			break;
		}
	}
	*/
	//InventoryTotalWeight += NewItem->GetItemStackWeight();
	RefreshInventory();
}

bool UAPInventoryComponent::MoveItemBetweenPanels(int32 SourceIndex, int32 DestinationIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("slot numbers : %d , item numbers : %d"), InventorySlotsCapacity, ItemNumbers);

	if (InventoryContents[SourceIndex]->ID == "NONE") return true;

	if (SourceIndex >= 0 && SourceIndex < InventorySlotsCapacity && DestinationIndex >= 0 && DestinationIndex < InventorySlotsCapacity)
	{
		TObjectPtr<UAPItemBase> tmp = InventoryContents[SourceIndex];
		InventoryContents[SourceIndex] = InventoryContents[DestinationIndex];
		InventoryContents[DestinationIndex] = tmp;

		return true;
	}
	return false;
}

void UAPInventoryComponent::SortingInventory()
{
	//if (ItemNumbers == 0) return;

	UE_LOG(LogTemp, Warning, TEXT("아이템 개수 : %d"), InventoryContents.Num());
	if (ItemNumbers == 0) return;

	for (int i = InventoryContents.Num() - 1; i > 0; i--)
	{

		//UE_LOG(LogTemp, Warning, TEXT("%s"), *InventoryContents[i]->ID.ToString());

		for (int j = 0; j < i; j++)
		{
			bool IsActive = false;
			const int RangeType = SortingTimes % 3;
			if (RangeType == 0) IsActive = InventoryContents[j]->ItemType < InventoryContents[j + 1]->ItemType;
			else if (RangeType == 1) IsActive = InventoryContents[j]->ItemQuality < InventoryContents[j + 1]->ItemQuality;
			else if (RangeType == 2) IsActive = InventoryContents[j]->ItemNumericData.Cost < InventoryContents[j + 1]->ItemNumericData.Cost;

			if (IsActive)
			{
				MoveItemBetweenPanels(j, j + 1);
			}
		}
	}

	SortingTimes++;
	RefreshInventory();
}

int32 UAPInventoryComponent::GetIndexOfItem(UAPItemBase* Slot)
{
	int32 SlotIndex = InventoryContents.Find(Slot);

	return SlotIndex;
}

void UAPInventoryComponent::SetIndexOfHoveredItem(UAPItemBase* HoveredItem)
{
	int32 SlotIndex = InventoryContents.Find(HoveredItem);

	HoveredItemIndex = SlotIndex;
}

void UAPInventoryComponent::RefreshInventory()
{
	OnInventoryUpdated.Broadcast();
}

// Minhyeong
UAPItemBase * UAPInventoryComponent::FindItembyId(FName DesiredItemID) const
{
	if(InventoryContents.IsEmpty()) return nullptr;

	for(UAPItemBase* APItemBase : InventoryContents)
	{
		if(APItemBase->ID == DesiredItemID)
		{
			return APItemBase;
		}
	}
	return nullptr;
}
