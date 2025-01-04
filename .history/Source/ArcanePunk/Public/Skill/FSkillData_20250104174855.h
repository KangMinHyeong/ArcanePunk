// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Common/APCrowdControlComponent.h"
#include "Engine/DataTable.h"
#include "FSkillData.generated.h"

/**
 * 
 */

class UNiagaraSystem;

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	Projectile = 0,
	Immediate = 1,
};

USTRUCT(BlueprintType)
struct ARCANEPUNK_API FSkillData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillType SkillType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterState CrowdControl  = ECharacterState::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UNiagaraSystem> SkillEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UNiagaraSystem> SkillOffEffect; // 사라지거나 파괴될 때 스폰되는 이펙트
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UShapeComponent> CollisionShapeClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector CollisionSize;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillSoundPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProjectileSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LaunchAngle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAttachedEffect = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDrag = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 MPConsumption;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 PenetrateCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SpecialEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* SkillAction;
};

USTRUCT(BlueprintType)
struct ARCANEPUNK_API FSkillAbilityData : public FTableRowBase // 스킬 강화 데이터
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName  AbilityName; // Ability 중첩 수
	
	UPROPERTY(EditAnywhere, meta = (MultiLine = true))
	FString  AbilityInformation; // Ability 설명명

	UPROPERTY(EditAnywhere)
	UTexture2D* AbilitySlotImage;

	UPROPERTY(EditAnywhere)
	uint8 MaxNesting = 1;
	
	UPROPERTY(EditAnywhere)
	int32 Price = 0;
	
};

// USTRUCT(BlueprintType)
// struct ARCANEPUNK_API FSkillAbilityNestingData : public FTableRowBase // 스킬 강화 중첩 데이터터
// {
// 	GENERATED_USTRUCT_BODY()

// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	TMap<FName, uint8>  SkillAbilityNestingData; // Ability 이름 / 중첩 수수
	
	
// };



// 추가해야하는 목록
// 데미지 계수
// 이펙트 Attach인지 아닌지
// 즉발인지 아닌지