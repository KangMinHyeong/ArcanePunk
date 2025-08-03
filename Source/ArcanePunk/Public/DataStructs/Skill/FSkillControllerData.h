#pragma once
#include "DataStructs/FValidatableTableRowBase.h"
#include "FSkillControllerData.generated.h"

UENUM(BlueprintType)
enum class ESkillGrade : uint8
{
	Normal = 0,
	Rare = 1,
	Epic = 2,
	Legendary = 3
};

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	Projectile = 0,
	Immediate = 1,
	Installation = 2,
	Buff = 3,
	Charging = 4,
	Normal = 0,
	Rare = 1,
	Epic = 2,
	Legendary = 3
};

USTRUCT(BlueprintType)
struct ARCANEPUNK_API FSkillControllerData : public FValidatableTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Memo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SkillID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillType SkillType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillGrade SkillGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AniMontageID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SkillEffectID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsStack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int32 StackCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCombo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NextComboSkillID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int32 StackValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int32 CoolDownTime_msec;
}; 