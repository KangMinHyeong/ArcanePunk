#pragma once
#include "DataStructs/FValidatableTableRowBase.h"
#include "Engine/DataTable.h"
#include "DataStructs/Item/APItemDataStruct.h"
#include "FItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemData : public FValidatableTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemQuality ItemQuality;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemNumericData ItemNumericData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemAssetData ItemAssetData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemStatistics ItemStatistics;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemTextData ItemTextData;
}; 