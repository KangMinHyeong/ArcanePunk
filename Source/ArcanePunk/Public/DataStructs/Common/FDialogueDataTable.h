#pragma once
#include "DataStructs/FValidatableTableRowBase.h"
#include "FDialogueDataTable.generated.h"

USTRUCT(BlueprintType)
struct FDialogueDataTable : public FValidatableTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Diologue_ID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Diologue_Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Portrait_Type = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEmphasize = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Character_ID = 0;
};

USTRUCT(BlueprintType)
struct FDialogueGroupData
{
	GENERATED_BODY()

	FDialogueGroupData() {}

	UPROPERTY()
	TArray<FDialogueDataTable> DialogueGroupData;
}; 