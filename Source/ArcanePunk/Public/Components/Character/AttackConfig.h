// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Common/APCrowdControlComponent.h"
#include "AttackConfig.generated.h"

/**
 * 공격 설정을 담는 구조체
 * 데미지, 넉백, CC, DOT 등 모든 공격 타입을 표현
 */
USTRUCT(BlueprintType)
struct FAttackConfig
{
	GENERATED_BODY()

	// ========== 기본 공격 파라미터 ==========

	/** 공격 시작 위치 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Basic")
	FVector StartLocation = FVector::ZeroVector;

	/** 공격 종료 위치 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Basic")
	FVector EndLocation = FVector::ZeroVector;

	/** 공격 반경 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Basic")
	float Radius = 100.0f;

	/** 데미지 배율 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Basic")
	float DamageMultiplier = 1.0f;

	/** 히트 횟수 (다단히트) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Basic")
	uint8 HitCount = 1;

	/** 즉사 확률 (0.0 ~ 1.0) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Basic")
	float InstantKillChance = 0.0f;

	// ========== Crowd Control (CC) ==========

	/** CC 적용 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|CrowdControl")
	bool bApplyCrowdControl = false;

	/** CC 타입 (Stun, Freeze, Slow 등) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|CrowdControl", meta = (EditCondition = "bApplyCrowdControl"))
	ECharacterState CrowdControlType = ECharacterState::None;

	/** CC 지속 시간 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|CrowdControl", meta = (EditCondition = "bApplyCrowdControl"))
	float CrowdControlDuration = 0.0f;

	// ========== 넉백 (Knock Back) ==========

	/** 넉백 적용 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|KnockBack")
	bool bApplyKnockBack = false;

	/** 넉백 거리 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|KnockBack", meta = (EditCondition = "bApplyKnockBack"))
	float KnockBackDistance = 0.0f;

	/** 넉백 지속 시간 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|KnockBack", meta = (EditCondition = "bApplyKnockBack"))
	float KnockBackDuration = 0.0f;

	/** 플레이어도 넉백 받는지 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|KnockBack", meta = (EditCondition = "bApplyKnockBack"))
	bool bKnockBackPlayer = false;

	// ========== 도트 데미지 (DOT - Damage Over Time) ==========

	/** 화상 (Burn) 적용 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|DOT")
	bool bApplyBurn = false;

	/** 화상 초당 데미지 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|DOT", meta = (EditCondition = "bApplyBurn"))
	float BurnDamagePerSecond = 0.0f;

	/** 화상 총 지속 시간 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|DOT", meta = (EditCondition = "bApplyBurn"))
	float BurnDuration = 0.0f;

	/** 화상 틱 간격 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|DOT", meta = (EditCondition = "bApplyBurn"))
	float BurnTickRate = 1.0f;

	// ========== 슬로우 (Slow) ==========

	/** 슬로우 적용 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Slow")
	bool bApplySlow = false;

	/** 슬로우 비율 (0 ~ 100) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Slow", meta = (EditCondition = "bApplySlow", ClampMin = "0", ClampMax = "100"))
	int32 SlowPercent = 0;

	/** 슬로우 지속 시간 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Slow", meta = (EditCondition = "bApplySlow"))
	float SlowDuration = 0.0f;

	// ========== 고급 옵션 ==========

	/** 플레이어 제외 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Advanced")
	bool bExcludePlayer = true;

	/** 커스텀 반경 사용 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Advanced")
	bool bUseCustomRadius = false;

	/** 커스텀 반경 값 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Advanced", meta = (EditCondition = "bUseCustomRadius"))
	float CustomRadius = 0.0f;

	/** 근접 공격 여부 (레이캐스트 방식 변경) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Advanced")
	bool bIsCloseRangeAttack = false;

	// ========== 헬퍼 함수 ==========

	/** 실제 사용할 반경 반환 */
	FORCEINLINE float GetEffectiveRadius() const
	{
		return bUseCustomRadius ? CustomRadius : Radius;
	}

	/** CC가 활성화되어 있는지 확인 */
	FORCEINLINE bool HasAnyCrowdControl() const
	{
		return bApplyCrowdControl || bApplyKnockBack || bApplySlow || bApplyBurn;
	}

	/** 디버그 정보 출력 */
	FString ToString() const
	{
		return FString::Printf(TEXT("AttackConfig[Radius=%.1f, Mult=%.2f, Hits=%d, CC=%s]"),
			Radius, DamageMultiplier, HitCount,
			HasAnyCrowdControl() ? TEXT("Yes") : TEXT("No"));
	}
};


/**
 * 공격 결과를 담는 구조체
 */
USTRUCT(BlueprintType)
struct FAttackResult
{
	GENERATED_BODY()

	/** 피격된 액터 목록 */
	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	TArray<AActor*> HitActors;

	/** 총 데미지량 */
	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	float TotalDamageDealt = 0.0f;

	/** 크리티컬 히트 수 */
	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	int32 CriticalHitCount = 0;

	/** 즉사 처치 수 */
	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	int32 InstantKillCount = 0;

	/** 성공 여부 */
	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	bool bSuccessful = false;

	/** 디버그 정보 출력 */
	FString ToString() const
	{
		return FString::Printf(TEXT("AttackResult[Hits=%d, Damage=%.1f, Crits=%d, Kills=%d]"),
			HitActors.Num(), TotalDamageDealt, CriticalHitCount, InstantKillCount);
	}
};
