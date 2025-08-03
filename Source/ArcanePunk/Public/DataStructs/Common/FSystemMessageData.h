#pragma once
#include "DataStructs/FValidatableTableRowBase.h"
#include "FSystemMessageData.generated.h"

USTRUCT(BlueprintType)
struct FSystemMessageData : public FValidatableTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID = 0;

	FString GetText() const { return Text; }
	int32 GetID() const { return ID; }
}; 