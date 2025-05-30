// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDropOperation.generated.h"

class UAPInventoryComponent;
class UAPItemBase;

UCLASS()
class ARCANEPUNK_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()


public:
	UPROPERTY()
	UAPItemBase* SourceItem;

	UPROPERTY()
	UAPInventoryComponent* SourceInventory;

};
