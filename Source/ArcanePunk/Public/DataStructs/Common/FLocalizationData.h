#pragma once
#include "DataStructs/FValidatableTableRowBase.h"
#include "Engine/DataTable.h"
#include "FLocalizationData.generated.h"

USTRUCT(BlueprintType)
struct FLocalizationData : public FValidatableTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Content;
};