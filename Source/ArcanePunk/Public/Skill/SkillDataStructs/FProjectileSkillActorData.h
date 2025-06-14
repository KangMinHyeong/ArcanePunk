// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Common/APCrowdControlComponent.h"
#include "Engine/DataTable.h"
#include "Skill/SkillDataStructs/FSkillActorData.h"
#include "Skill/EAttackType.h"
#include "FProjectileSkillActorData.generated.h"

class AAPSkillRange;
class UNiagaraSystem;
class UTexture2D;

USTRUCT(BlueprintType)
struct ARCANEPUNK_API FProjectileSkillActorData : public FSkillActorData
{
	GENERATED_USTRUCT_BODY()
	
	// 메모용 필드 (작업자 설명 등)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Memo;

	// 스킬에 사용할 VFX 에셋 (Niagara 시스템 경로)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UNiagaraSystem> SkillEffect;

	// 스킬의 속성 유형 (예: 물리, 아케인 등)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttackType AttackType;

	// 스킬 사운드 (SoundCue 또는 SoundWave)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundBase> SoundAsset;

	// 적용할 데미지 공식의 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DamageID;

	// 한 번의 공격 판정에 몇 번 타격할지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1"))
	int32 HitCount;

	// 투사체 수량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1"))
	int32 ProjectileQuantity;

	// 투사체 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1"))
	int32 ProjectileSpeed;

	// 투사체 지속 시간 (밀리초 단위)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1"))
	int32 ProjectileDuration_msec;

	// 투사체 발사 각도 (0~360도, 0이면 같은 방향)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "360"))
	int32 ProjectileLaunchAngle;

	// 관통 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsPenetration;

	// 관통 가능한 수 (1 이상)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1"))
	int32 PenetrationCount;

	// 유도 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsGuided;

	// 충돌체로 사용할 컴포넌트 클래스 (Sphere, Box, Capsule 등)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UShapeComponent> CollisionShapeClass;

	// 충돌체 크기 (사용된 ShapeComponent에 따라 해석 방식이 달라짐)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SizeVector;
};