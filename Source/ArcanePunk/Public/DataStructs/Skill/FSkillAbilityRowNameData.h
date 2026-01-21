#pragma once
#include "DataStructs/FValidatableTableRowBase.h"
#include "Engine/DataTable.h"
#include "FSkillAbilityRowNameData.generated.h"

USTRUCT(BlueprintType)
struct FSkillAbilityRowNameData : public FValidatableTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> SilverRowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> GoldRowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> PlatinumRowName;
}; 