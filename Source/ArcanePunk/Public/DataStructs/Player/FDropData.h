#pragma once
#include "DataStructs/FValidatableTableRowBase.h"
#include "Engine/DataTable.h"
#include "FDropData.generated.h"

USTRUCT(BlueprintType)
struct FDropData : public FValidatableTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DropArcaneEnergy_Percent = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DropArcaneEnergy_Min = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DropArcaneEnergy_Max = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DropGold_Percent = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DropGold_Min = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DropGold_Max = 0;
}; 