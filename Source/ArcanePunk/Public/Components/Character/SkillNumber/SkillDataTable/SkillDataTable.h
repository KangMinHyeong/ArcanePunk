// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SkillDataTable.generated.h"

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
	Skill_14 = 14 UMETA(DisplayName = "스킬 No.14")
};

UENUM(BlueprintType)
enum class EUltSkillNumber : uint8 // 궁극기 스킬 넘버
{
	None	 	= 0 UMETA(DisplayName = "None"),
	UltSkill_1	= 1 UMETA(DisplayName = "궁극기 스킬 No.1 : Arcane Rain"),
};

UENUM(BlueprintType)
enum class ESkillTypeState : uint8 // 스킬 타입
{
	Type_None	= 0,
    Type_Q      = 1, // 빙결
    Type_E      = 2, // 화염
    Type_R	 	= 3, // 전기
};

UENUM(BlueprintType)
enum class ESkillAbility : uint8 // 스킬 추가 능력
{
	Ability_None	= 0,
    Gigant			= 1 UMETA(DisplayName = "거대화"), // 스킬 범위 증가
    Homing			= 2 UMETA(DisplayName = "유도"), // 유도
    Stun		 	= 3 UMETA(DisplayName = "스턴"), // CC기(스턴)
};

UENUM(BlueprintType)
enum class ESkillKey : uint8
{
	None		= 0,
    Q			= 1,
    E			= 2,
    R		 	= 3, 
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
enum class ESkillRangeType : uint8 // 스킬 범위표시 타입
{
	None	 		= 0 UMETA(DisplayName = "None"),
	Control_Circle	= 1 UMETA(DisplayName = "논타겟 원형"), 			 // ex) 제라스 W
	Arrow			= 2 UMETA(DisplayName = "직선형"), 					// ex) 이즈리얼 Q
	Around_Circle	= 3 UMETA(DisplayName = "플레이어 기준 범위형 원형"), //
};


UCLASS()
class ARCANEPUNK_API USkillDataTable : public UDataTable
{
	GENERATED_BODY()
	
};
