#pragma once
#include "DataStructs/FValidatableTableRowBase.h"
#include "Engine/DataTable.h"
#include "FSkillNameList.generated.h"

USTRUCT(BlueprintType)
struct FSkillNameList : public FValidatableTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillName_Korean;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Skill_Information;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* SkillSlotImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MPConsumption = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Price = 0;
}; 