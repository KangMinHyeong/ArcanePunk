#pragma once
#include "DataStructs/FValidatableTableRowBase.h"
#include "Engine/DataTable.h"
#include "FCharaterUIData.generated.h"

USTRUCT(BlueprintType)
struct FCharaterUIData : public FValidatableTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	FCharaterUIData() {}

	UPROPERTY(EditAnywhere)
	FString CharaterName;

	UPROPERTY(EditAnywhere)
	TMap<uint8, UTexture2D*> Portraits;
}; 