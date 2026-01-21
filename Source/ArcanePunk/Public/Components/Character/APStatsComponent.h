// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/Common/ICharacterComponent.h"
#include "PlayerState/APPlayerData.h"
#include "GameState/APGameState.h"
#include "APStatsComponent.generated.h"

class AAPCharacterBase;

// 스탯 변경 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, CurrentHP, float, MaxHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnManaChanged, int32, CurrentMP, int32, MaxMP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AAPCharacterBase*, DeadCharacter);

/**
 * 캐릭터의 모든 스탯(HP, MP, ATK, DEF, Speed 등)을 관리하는 컴포넌트
 * BaseStats(영구 스탯)과 CurrentStats(버프/디버프 적용 스탯)을 분리하여 관리
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPStatsComponent : public UActorComponent, public ICharacterComponent
{
	GENERATED_BODY()

public:
	UAPStatsComponent();

protected:
	virtual void BeginPlay() override;

public:
	// ========== ICharacterComponent Interface ==========
	virtual void InitializeComponent(AAPCharacterBase* OwnerCharacter) override;
	virtual void SetComponentEnabled(bool bEnabled) override;
	virtual bool IsComponentEnabled() const override;

	// ========== Health (HP) Management ==========

	/** 현재 HP 반환 */
	UFUNCTION(BlueprintPure, Category = "Stats")
	float GetCurrentHealth() const { return CurrentStats.StatusData.HP; }

	/** 최대 HP 반환 */
	UFUNCTION(BlueprintPure, Category = "Stats")
	float GetMaxHealth() const { return CurrentStats.StatusData.MaxHP; }

	/** HP 비율 (0.0 ~ 1.0) */
	UFUNCTION(BlueprintPure, Category = "Stats")
	float GetHealthPercent() const;

	/**
	 * HP 변경 (데미지 또는 힐)
	 * @param Delta - 변경량 (음수: 데미지, 양수: 힐)
	 * @return 실제 적용된 변경량
	 */
	UFUNCTION(BlueprintCallable, Category = "Stats")
	float ModifyHealth(float Delta);

	/** HP를 직접 설정 */
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetHealth(float NewHealth);

	/** 최대 HP 설정 */
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetMaxHealth(float NewMaxHealth);

	/** 사망 여부 체크 */
	UFUNCTION(BlueprintPure, Category = "Stats")
	bool IsDead() const { return GetCurrentHealth() <= 0.0f; }

	// ========== Mana (MP) Management ==========

	/** 현재 MP 반환 */
	UFUNCTION(BlueprintPure, Category = "Stats")
	int32 GetCurrentMana() const { return CurrentStats.StatusData.MP; }

	/** 최대 MP 반환 */
	UFUNCTION(BlueprintPure, Category = "Stats")
	int32 GetMaxMana() const { return CurrentStats.StatusData.MaxMP; }

	/** MP 비율 (0.0 ~ 1.0) */
	UFUNCTION(BlueprintPure, Category = "Stats")
	float GetManaPercent() const;

	/**
	 * MP 변경
	 * @param Delta - 변경량 (음수: 소모, 양수: 회복)
	 * @return 실제 적용된 변경량
	 */
	UFUNCTION(BlueprintCallable, Category = "Stats")
	int32 ModifyMana(int32 Delta);

	/** MP를 직접 설정 */
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetMana(int32 NewMana);

	/** 최대 MP 설정 */
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetMaxMana(int32 NewMaxMana);

	// ========== Combat Stats ==========

	/** 공격력 반환 */
	UFUNCTION(BlueprintPure, Category = "Stats")
	float GetAttackPower() const { return CurrentStats.StatusData.ATK; }

	/** 공격력 설정 */
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetAttackPower(float NewATK);

	/** 방어력 반환 */
	UFUNCTION(BlueprintPure, Category = "Stats")
	float GetDefense() const { return CurrentStats.StatusData.DEF; }

	/** 방어력 설정 */
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetDefense(float NewDEF);

	/** 이동 속도 반환 */
	UFUNCTION(BlueprintPure, Category = "Stats")
	float GetMoveSpeed() const { return CurrentStats.StatusData.MoveSpeed; }

	/** 이동 속도 설정 */
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetMoveSpeed(float NewSpeed);

	/** 크리티컬 확률 반환 */
	UFUNCTION(BlueprintPure, Category = "Stats")
	float GetCriticalChance() const { return CurrentStats.StatusData.CriticalPercent; }

	/** 크리티컬 데미지 계수 반환 */
	UFUNCTION(BlueprintPure, Category = "Stats")
	float GetCriticalDamageMultiplier() const { return CurrentStats.StatusData.CriticalDamageCoefficient; }

	// ========== Stat Modifiers (Buffs/Debuffs) ==========

	/**
	 * 기본 스탯 반환 (버프/디버프 적용 전)
	 * 영구적 스탯 증가 (레벨업 등)
	 */
	UFUNCTION(BlueprintPure, Category = "Stats")
	const FPlayerTotalData& GetBaseStats() const { return BaseStats; }

	/**
	 * 현재 스탯 반환 (버프/디버프 적용 후)
	 * 일시적 스탯 변경 (장비, 버프 등)
	 */
	UFUNCTION(BlueprintPure, Category = "Stats")
	const FPlayerTotalData& GetCurrentStats() const { return CurrentStats; }

	/**
	 * 기본 스탯 설정 (영구 변경)
	 * 레벨업, 스탯 포인트 투자 등에 사용
	 */
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetBaseStats(const FPlayerTotalData& NewStats);

	/**
	 * 현재 스탯을 기본 스탯으로 리셋
	 * 모든 버프/디버프 제거
	 */
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void ResetToBaseStats();

	/**
	 * 스탯 업데이트 강제 실행
	 * 버프/디버프 재계산
	 */
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void UpdateStats();

public:
	// ========== Delegates ==========

	/** HP 변경 시 호출 */
	UPROPERTY(BlueprintAssignable, Category = "Stats|Events")
	FOnHealthChanged OnHealthChanged;

	/** MP 변경 시 호출 */
	UPROPERTY(BlueprintAssignable, Category = "Stats|Events")
	FOnManaChanged OnManaChanged;

	/** 사망 시 호출 */
	UPROPERTY(BlueprintAssignable, Category = "Stats|Events")
	FOnDeath OnDeath;

protected:
	/** 소유자 캐릭터 */
	UPROPERTY()
	TWeakObjectPtr<AAPCharacterBase> OwnerCharacter;

	/** 컴포넌트 활성화 여부 */
	UPROPERTY()
	bool bComponentEnabled = true;

	/** 기본 스탯 (영구) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FPlayerTotalData BaseStats;

	/** 현재 스탯 (버프/디버프 적용) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FPlayerTotalData CurrentStats;

	/** 게임 데이터 (레벨, 경험치 등) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FGameData GameData;

private:
	/** HP 클램핑 (0 ~ MaxHP) */
	void ClampHealth();

	/** MP 클램핑 (0 ~ MaxMP) */
	void ClampMana();
};
