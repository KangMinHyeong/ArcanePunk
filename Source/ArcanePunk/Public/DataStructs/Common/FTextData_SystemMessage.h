#pragma once
#include "DataStructs/FValidatableTableRowBase.h"
#include "FTextData_SystemMessage.generated.h"

USTRUCT(BlueprintType)
struct FTextData_SystemMessage : public FValidatableTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID = 0;

	FString GetText() const { return Text; }
	int32 GetID() const { return ID; }
}; 