// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DataStructs/Skill/FSkillAbilityData.h"
#include "DataStructs/Skill/FSkillNameList.h"
#include "DataStructs/Skill/FSkillAbilityRowNameData.h"
#include "SkillDataTable.generated.h"


class UTexture2D;

UENUM(BlueprintType)
enum class ESkillNumber : uint8 // 스킬 넘버
{
	None 	= 0 UMETA(DisplayName = "None"),
	Skill_1 = 1 UMETA(DisplayName = "스킬 No.1"),
	Skill_2 = 2 UMETA(DisplayName = "스킬 No.2"),
	Skill_3 = 3 UMETA(DisplayName = "스킬 No.3"),
	Skill_4 = 4 UMETA(DisplayName = "스킬 No.4"),
	Skill_5 = 5 UMETA(DisplayName = "스킬 No.5"),
	Skill_6 = 6 UMETA(DisplayName = "스킬 No.6"),
	Skill_7 = 7 UMETA(DisplayName = "스킬 No.7"),
	Skill_8 = 8 UMETA(DisplayName = "스킬 No.8"),
	Skill_9 = 9 UMETA(DisplayName = "스킬 No.9"),
	Skill_10 = 10 UMETA(DisplayName = "스킬 No.10"),
	Skill_11 = 11 UMETA(DisplayName = "스킬 No.11"),
	Skill_12 = 12 UMETA(DisplayName = "스킬 No.12"),
	Skill_13 = 13 UMETA(DisplayName = "스킬 No.13"),
	Skill_14 = 14 UMETA(DisplayName = "스킬 No.14"),
	Skill_15 = 15 UMETA(DisplayName = "스킬 No.15"),
	Skill_16 = 16 UMETA(DisplayName = "스킬 No.16"),
	Skill_17 = 17 UMETA(DisplayName = "스킬 No.17"),
	Skill_18 = 18 UMETA(DisplayName = "스킬 No.18"),
	Skill_19 = 19 UMETA(DisplayName = "스킬 No.19"),
	Skill_20 = 20 UMETA(DisplayName = "스킬 No.20"),
};

UENUM(BlueprintType)
enum class EUltSkillNumber : uint8 // 궁극기 스킬 넘버
{
	None	 	= 0 UMETA(DisplayName = "None"),
	UltSkill_1	= 1 UMETA(DisplayName = "궁극기 스킬 No.1 : Arcane Rain"),
	UltSkill_2	= 2 UMETA(DisplayName = "궁극기 스킬 No.2 : Time Rewind"),
	UltSkill_3	= 3 UMETA(DisplayName = "궁극기 스킬 No.3 : Arcane Execution"),
	UltSkill_4	= 4 UMETA(DisplayName = "궁극기 스킬 No.4 : Black Hole"),
	UltSkill_5	= 5 UMETA(DisplayName = "궁극기 스킬 No.5 : Shadow Shuriken"),
	UltSkill_6	= 6 UMETA(DisplayName = "궁극기 스킬 No.6 : Wind Rush"),
	UltSkill_7	= 7 UMETA(DisplayName = "궁극기 스킬 No.7 : Arcane TelePort"),
	UltSkill_8	= 8 UMETA(DisplayName = "궁극기 스킬 No.8 : Arcane Meteor"),
	UltSkill_9	= 9 UMETA(DisplayName = "궁극기 스킬 No.9 : Carpet Boom"),
	UltSkill_10	= 10 UMETA(DisplayName = "궁극기 스킬 No.10 : Terminator"),
	UltSkill_11	= 11 UMETA(DisplayName = "궁극기 스킬 No.11 : Overload"),
	UltSkill_12	= 12 UMETA(DisplayName = "궁극기 스킬 No.12 : Arcane Reflector"),
	UltSkill_13	= 13 UMETA(DisplayName = "궁극기 스킬 No.13 : Arcane Wave"),
	UltSkill_14	= 14 UMETA(DisplayName = "궁극기 스킬 No.14 : Wind Cutter"),
	UltSkill_15	= 15 UMETA(DisplayName = "궁극기 스킬 No.15 : BomberMan"),
	UltSkill_16	= 16 UMETA(DisplayName = "궁극기 스킬 No.16 : Superior Mode"),
	UltSkill_17	= 17 UMETA(DisplayName = "궁극기 스킬 No.17 : Arcane Area"),
	UltSkill_18	= 18 UMETA(DisplayName = "궁극기 스킬 No.18 : Arcane Blast"),
	UltSkill_19	= 19 UMETA(DisplayName = "궁극기 스킬 No.19 : Arcane Alter Ego"),
	UltSkill_20	= 20 UMETA(DisplayName = "궁극기 스킬 No.20 : Imitator"),
};

UENUM(BlueprintType)
enum class EQSkill : uint8 // Q스킬
{
	None 			 = 0 UMETA(DisplayName = "None"),
	// Skill_1			 = 1 UMETA(DisplayName = "스킬 No.1"),
	// Skill_2 		 = 2 UMETA(DisplayName = "스킬 No.2"),
	Skill_6			 = 6 UMETA(DisplayName = "스킬 No.6"),
	Skill_8			 = 8 UMETA(DisplayName = "스킬 No.8"),
	Skill_9			 = 9 UMETA(DisplayName = "스킬 No.9"),
	// Skill_11		 = 11 UMETA(DisplayName = "스킬 No.11"),
	// Skill_14		 = 14 UMETA(DisplayName = "스킬 No.14"),
	// Skill_16 		 = 16 UMETA(DisplayName = "스킬 No.16"),
	// Skill_17		 = 17 UMETA(DisplayName = "스킬 No.17"),
	// Skill_18		 = 18 UMETA(DisplayName = "스킬 No.18"),
	// Skill_19		 = 19 UMETA(DisplayName = "스킬 No.19"),
};

UENUM(BlueprintType)
enum class EESkill : uint8 // E스킬
{
	None 			 = 0 UMETA(DisplayName = "None"),
	// Skill_3			 = 3 UMETA(DisplayName = "스킬 No.3"),
	// Skill_4			 = 4 UMETA(DisplayName = "스킬 No.4"),
	Skill_5 		 = 5 UMETA(DisplayName = "스킬 No.5"),
	Skill_7			 = 7 UMETA(DisplayName = "스킬 No.7"),
	Skill_10		 = 10 UMETA(DisplayName = "스킬 No.10"),
	// Skill_12		 = 12 UMETA(DisplayName = "스킬 No.12"),
	// Skill_13		 = 13 UMETA(DisplayName = "스킬 No.13"),
	// Skill_15 		 = 15 UMETA(DisplayName = "스킬 No.15"),
	// Skill_20		 = 20 UMETA(DisplayName = "스킬 No.20"),
};

UENUM(BlueprintType)
enum class EPassiveNumber : uint8 // 패시브 스킬
{
	None 			 = 0 UMETA(DisplayName = "None"),
	Passive_1		 = 1 UMETA(DisplayName = "패시브 No.1"),
	Passive_2		 = 2 UMETA(DisplayName = "패시브 No.2"),
	Passive_3 		 = 3 UMETA(DisplayName = "패시브 No.3"),
	Passive_4 		 = 4 UMETA(DisplayName = "패시브 No.4"),
	Passive_5 		 = 5 UMETA(DisplayName = "패시브 No.5"),
	Passive_6 		 = 6 UMETA(DisplayName = "패시브 No.6"),
	Passive_7 		 = 7 UMETA(DisplayName = "패시브 No.7"),
	Passive_8 		 = 8 UMETA(DisplayName = "패시브 No.8"),
	Passive_9 		 = 9 UMETA(DisplayName = "패시브 No.9"),
	Passive_10 		 = 10 UMETA(DisplayName = "패시브 No.10"),
};

UENUM(BlueprintType)
enum class ESkillTypeState : uint8 // 스킬 타입
{
	Type_None	= 0,
    Type_Q      = 1, // 빙결
    Type_E      = 2, // 화염
    Type_R	 	= 3, // 전기
};

// UENUM(BlueprintType)
// enum class ESkillAbility : uint8 // 스킬 추가 능력
// {
// 	Ability_None	= 0,
//     Gigant			= 1 UMETA(DisplayName = "거대화"), // 스킬 범위 증가
//     Homing			= 2 UMETA(DisplayName = "유도"), // 유도
//     Stun		 	= 3 UMETA(DisplayName = "스턴"), // CC기(스턴)
// };

UENUM(BlueprintType)
enum class ESkillKey : uint8
{
	None		= 0,
    Q			= 1,
    E			= 2,
    R		 	= 3, 
	Dash	 	= 4, 
	Parrying 	= 5, 
};

UENUM(BlueprintType)
enum class ESkillCursor : uint8 // 스킬 마우스커서
{
	Default		= 0,
    Crosshairs	= 1,
};

UENUM(BlueprintType)
enum class ESkillCategory : uint8 // 스킬 형태 카테고리
{
	None				= 0, 
    Projecitle			= 1, // 투사체(중력 영향 X)
    Laser				= 2, // 레이저(빔)
    Throw			 	= 3, // 투척물(중력 영향 O)
	InvisibleCollider	= 4, // 무형 콜랑이더
	Buff				= 5,  // 버프
	Spawn_Static		= 6,  // 소환물(고정)
	Spawn_Dynamic		= 7   // 소환물(동적)
};

UENUM(BlueprintType)
enum class EAbilityCalculation : uint8 // 패시브 스킬
{
	None 			 = 0 UMETA(DisplayName = "None"),
	Add				 = 1 UMETA(DisplayName = "합연산"), // 100 + 40
	Multiple		 = 2 UMETA(DisplayName = "곱연산"), // 100 * (1 + 0.2 * 2)
	Multiple_Nesting = 3 UMETA(DisplayName = "중첩 곱연산"), // 100 * 1.2 * 1.2
};

UENUM(BlueprintType)
enum class ESkillRangeType : uint8 // 스킬 범위표시 타입
{
	None	 		= 0 UMETA(DisplayName = "None"),
	Control_Circle	= 1 UMETA(DisplayName = "논타겟 원형"), 			 // ex) 제라스 W
	Arrow			= 2 UMETA(DisplayName = "직선형"), 					// ex) 이즈리얼 Q
	Around_Circle	= 3 UMETA(DisplayName = "플레이어 기준 범위형 원형"), // 
	Two_Circle		= 4 UMETA(DisplayName = "중심이 같고 반지름이 다른 두 원형"), // 직스 R
	Square			= 5 UMETA(DisplayName = "직사각형"), // 오른 R
	SectorCircle	= 6 UMETA(DisplayName = "부채꼴"), // 탈론 W
};

UENUM(BlueprintType)
enum class EEnhanceCategory : uint8 // 증강 타입 넘버
{
	None	 			= 0 UMETA(DisplayName = "None"),
	Enhance_Q			= 1 UMETA(DisplayName = "QSkill 스킬 강화"),
	Enhance_E			= 2 UMETA(DisplayName = "ESkill 스킬 강화"),
	Enhance_R			= 3 UMETA(DisplayName = "RSkill 스킬 강화"),
	Enhance_Passive		= 4 UMETA(DisplayName = "Passive 강화"),
	// Enhance_Dash		= 5 UMETA(DisplayName = "이동기 강화"),
	// Enhance_LeftMouse	= 6 UMETA(DisplayName = "LeftMouse 공격 강화"),
	// Enhance_RightMouse	= 7 UMETA(DisplayName = "RightMouse 공격 강화"),
};

UENUM(BlueprintType)
enum class EEnHanceType : uint8
{
	None		 = 0,
	Silver		 = 1,
    Gold		 = 2,
    Platinum     = 3,
};

USTRUCT()
struct FSkillAbilityList
{
	GENERATED_USTRUCT_BODY()

	FSkillAbilityList() {}
	
	UPROPERTY(EditAnywhere)
	FString AbilityName;

	UPROPERTY(EditAnywhere, meta = (MultiLine = true))
	FString AbilityInformation;
	
	UPROPERTY(EditAnywhere)
	bool Nestable = false;

	UPROPERTY(EditAnywhere)
	float Coefficient_X = 0.0f;

	UPROPERTY(EditAnywhere)
	float Coefficient_Y = -1.0f;
};



// USTRUCT()
// struct FSkillAbilityDataTable : public FTableRowBase
// {
// 	GENERATED_USTRUCT_BODY()

// 	FSkillAbilityDataTable() {}

// 	UPROPERTY(EditAnywhere)
// 	TMap<uint8, FSkillAbilityList> SilverAbilityInformation;

// 	UPROPERTY(EditAnywhere)
// 	TMap<uint8, FSkillAbilityList> GoldAbilityInformation;

// 	UPROPERTY(EditAnywhere)
// 	TMap<uint8, FSkillAbilityList> PlatinumAbilityInformation;
// };

USTRUCT()
struct FSkillAbilityNestingData
{
	GENERATED_USTRUCT_BODY()

	FSkillAbilityNestingData() {}

	UPROPERTY(EditAnywhere)
	FName SkillName;

	UPROPERTY(EditAnywhere)
	TMap<uint8, uint16> SilverAbilityNestingNum;

	UPROPERTY(EditAnywhere)
	TMap<uint8, uint16> GoldAbilityNestingNum;

	UPROPERTY(EditAnywhere)
	TMap<uint8, uint16> PlatinumAbilityNestingNum;
};

UCLASS()
class ARCANEPUNK_API USkillDataTable : public UDataTable
{
	GENERATED_BODY()
	
};
