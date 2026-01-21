// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Character/APStatsComponent.h"
#include "Character/APCharacterBase.h"

UAPStatsComponent::UAPStatsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAPStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	// 현재 스탯을 기본 스탯으로 초기화
	CurrentStats = BaseStats;
	ClampHealth();
	ClampMana();
}

// ========== ICharacterComponent Interface ==========

void UAPStatsComponent::InitializeComponent(AAPCharacterBase* InOwnerCharacter)
{
	OwnerCharacter = InOwnerCharacter;
}

void UAPStatsComponent::SetComponentEnabled(bool bEnabled)
{
	bComponentEnabled = bEnabled;
}

bool UAPStatsComponent::IsComponentEnabled() const
{
	return bComponentEnabled;
}

// ========== Health (HP) Management ==========

float UAPStatsComponent::GetHealthPercent() const
{
	const float MaxHP = GetMaxHealth();
	return MaxHP > 0.0f ? (GetCurrentHealth() / MaxHP) : 0.0f;
}

float UAPStatsComponent::ModifyHealth(float Delta)
{
	if (!bComponentEnabled || Delta == 0.0f)
	{
		return 0.0f;
	}

	const float OldHealth = GetCurrentHealth();
	const float NewHealth = OldHealth + Delta;
	SetHealth(NewHealth);

	const float ActualDelta = GetCurrentHealth() - OldHealth;
	return ActualDelta;
}

void UAPStatsComponent::SetHealth(float NewHealth)
{
	const float OldHealth = CurrentStats.StatusData.HP;
	CurrentStats.StatusData.HP = NewHealth;
	ClampHealth();

	// HP가 실제로 변경되었을 때만 이벤트 발생
	if (CurrentStats.StatusData.HP != OldHealth)
	{
		OnHealthChanged.Broadcast(GetCurrentHealth(), GetMaxHealth());

		// 사망 체크
		if (IsDead())
		{
			OnDeath.Broadcast(OwnerCharacter.Get());
		}
	}
}

void UAPStatsComponent::SetMaxHealth(float NewMaxHealth)
{
	const float OldMaxHP = CurrentStats.StatusData.MaxHP;
	CurrentStats.StatusData.MaxHP = FMath::Max(0.0f, NewMaxHealth);

	// 최대 HP가 변경되면 현재 HP도 조정
	ClampHealth();

	if (CurrentStats.StatusData.MaxHP != OldMaxHP)
	{
		OnHealthChanged.Broadcast(GetCurrentHealth(), GetMaxHealth());
	}
}

void UAPStatsComponent::ClampHealth()
{
	CurrentStats.StatusData.HP = FMath::Clamp(CurrentStats.StatusData.HP, 0.0f, GetMaxHealth());
}

// ========== Mana (MP) Management ==========

float UAPStatsComponent::GetManaPercent() const
{
	const int32 MaxMP = GetMaxMana();
	return MaxMP > 0 ? (static_cast<float>(GetCurrentMana()) / MaxMP) : 0.0f;
}

int32 UAPStatsComponent::ModifyMana(int32 Delta)
{
	if (!bComponentEnabled || Delta == 0)
	{
		return 0;
	}

	const int32 OldMana = GetCurrentMana();
	const int32 NewMana = OldMana + Delta;
	SetMana(NewMana);

	const int32 ActualDelta = GetCurrentMana() - OldMana;
	return ActualDelta;
}

void UAPStatsComponent::SetMana(int32 NewMana)
{
	const int32 OldMana = CurrentStats.StatusData.MP;
	CurrentStats.StatusData.MP = NewMana;
	ClampMana();

	// MP가 실제로 변경되었을 때만 이벤트 발생
	if (CurrentStats.StatusData.MP != OldMana)
	{
		OnManaChanged.Broadcast(GetCurrentMana(), GetMaxMana());
	}
}

void UAPStatsComponent::SetMaxMana(int32 NewMaxMana)
{
	const int32 OldMaxMP = CurrentStats.StatusData.MaxMP;
	CurrentStats.StatusData.MaxMP = FMath::Max(0, NewMaxMana);

	// 최대 MP가 변경되면 현재 MP도 조정
	ClampMana();

	if (CurrentStats.StatusData.MaxMP != OldMaxMP)
	{
		OnManaChanged.Broadcast(GetCurrentMana(), GetMaxMana());
	}
}

void UAPStatsComponent::ClampMana()
{
	CurrentStats.StatusData.MP = FMath::Clamp(CurrentStats.StatusData.MP, 0, GetMaxMana());
}

// ========== Combat Stats ==========

void UAPStatsComponent::SetAttackPower(float NewATK)
{
	CurrentStats.StatusData.ATK = FMath::Max(0.0f, NewATK);
}

void UAPStatsComponent::SetDefense(float NewDEF)
{
	CurrentStats.StatusData.DEF = FMath::Max(0.0f, NewDEF);
}

void UAPStatsComponent::SetMoveSpeed(float NewSpeed)
{
	CurrentStats.StatusData.MoveSpeed = FMath::Max(0.0f, NewSpeed);
}

// ========== Stat Modifiers ==========

void UAPStatsComponent::SetBaseStats(const FPlayerTotalData& NewStats)
{
	BaseStats = NewStats;
	UpdateStats();
}

void UAPStatsComponent::ResetToBaseStats()
{
	CurrentStats = BaseStats;
	ClampHealth();
	ClampMana();

	// 변경 이벤트 브로드캐스트
	OnHealthChanged.Broadcast(GetCurrentHealth(), GetMaxHealth());
	OnManaChanged.Broadcast(GetCurrentMana(), GetMaxMana());
}

void UAPStatsComponent::UpdateStats()
{
	// 현재는 단순히 기본 스탯 복사
	// 추후 버프/디버프 시스템 추가 시 여기서 계산
	CurrentStats = BaseStats;
	ClampHealth();
	ClampMana();

	// 변경 이벤트 브로드캐스트
	OnHealthChanged.Broadcast(GetCurrentHealth(), GetMaxHealth());
	OnManaChanged.Broadcast(GetCurrentMana(), GetMaxMana());
}
