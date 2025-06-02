#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
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
struct ARCANEPUNK_API FSkillControllerData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	// 메모 컬럼(코드에서 사용 X)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient)
	FString Memo;

	// 스킬 고유 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SkillID;

	// 스킬 고유 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillType SkillType;
	
	// 스킬 명
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillName;

	// 스킬 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillDescription;

	// 스킬 등급
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillGrade SkillGrade;

	// 애니메이션 몽타주 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AniMontageID;

	// 스택형 스킬 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsStack;

	// 스택 값 (한 쿨타임 동안 사용 가능한 횟수)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StackValue;

	// 스킬 쿨타임 (밀리초 단위)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CoolDownTimeMsec;
};