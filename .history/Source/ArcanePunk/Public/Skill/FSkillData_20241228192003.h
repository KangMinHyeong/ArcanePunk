// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
};

USTRUCT(BlueprintType)
struct ARCANEPUNK_API FSkillData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillType SkillType;
	
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
struct ARCANEPUNK_API FSkillAbilityNestingData : public FTableRowBase // 스킬 강화 중첩 데이터터
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 DamageModuleNumber = 0;
	
	
};

// 추가해야하는 목록
// 데미지 계수