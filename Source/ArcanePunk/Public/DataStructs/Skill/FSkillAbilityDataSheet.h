#pragma once
#include "DataStructs/FValidatableTableRowBase.h"
#include "Engine/DataTable.h"
#include "FSkillAbilityDataSheet.generated.h"

USTRUCT(BlueprintType)
struct FSkillAbilityDataSheet : public FValidatableTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AbilityName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AbilityInformation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Nestable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Coefficient_X = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Coefficient_Y = -1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxNesting = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* EnhanceSlotImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Price = 0;
}; 