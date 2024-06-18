// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameState/APGameState.h"
#include "PlayerState/APPlayerData.h"
#include "Components/Character/SkillNumber/SkillDataTable/SkillDataTable.h"
#include "Components/Common/APCrowdControlComponent.h"
#include "GameFramework/Character.h"
#include "APCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateOriginStatus);
DECLARE_MULTICAST_DELEGATE(FOnCrowdControlCheck);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillTrigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillEndTrigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillEndWithSkillKey, ESkillKey, SkillKey, ESkillNumber, SkillNumber);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillChargingTrigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillEnhanceTrigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillRotationTrigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMaxHPAndHPChanged, float, MaxHP, float, HP);

class UAPMovementComponent;
class UNiagaraComponent;
class UAPAttackComponent;
class UAPHitPointComponent;
class UAPAnimHubComponent;

UCLASS()
class ARCANEPUNK_API AAPCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AAPCharacterBase();

protected:
	virtual void BeginPlay() override;

public:	
	FORCEINLINE UAPMovementComponent* GetAPMoveComponent() const {return MoveComponent;}; // MoveComp 반환
	FORCEINLINE UAPCrowdControlComponent* GetCrowdControlComp() const { return CrowdControlComp;};

	FORCEINLINE UMaterialInterface* GetDefaultMaterial() const {return DefaultMaterial;}; // DefaultMaterial 반환

	FORCEINLINE UAPHitPointComponent* GetHitPointComponent() const {return HitPointComponent;}; // HitPointComp 반환

	// 무적
	FORCEINLINE bool IsBlockMode() const {return bBlockMode;};

	// 캐릭터 현재 상태 // CC여부
	UFUNCTION(BlueprintPure)
	FORCEINLINE ECharacterState returnState() const {return CurrentState;}; // CurrentState 반환
	FORCEINLINE void SetState(ECharacterState UpdateState) { CurrentState = UpdateState;}; // CurrentState 설정
	FORCEINLINE UNiagaraComponent* GetStunEffect() const { return StunEffectComp;};
	FORCEINLINE UNiagaraComponent* GetFrozenEffect() const { return FrozenEffectComp;};

	// Hit 관련
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsHitting() const {return bHitting;};  // bHitting 반환
	FORCEINLINE void SetHitting(bool NewBool) {bHitting = NewBool;}; // bHitting 설정

	UFUNCTION(BlueprintPure)
	virtual bool IsDead();

	FORCEINLINE void SetDoing(bool NewBool) {bDoing = NewBool;}; // bDoing 설정
	FORCEINLINE bool GetDoing() const {return bDoing;}; // bDoing 반환

	//플레이어 스테이터스 관련 함수
	FORCEINLINE FPlayerTotalData GetPlayerStatus_Origin() const {return MyPlayerTotalStatus_Origin;}; // MyPlayerTotalStatus 반환
	FORCEINLINE void SetPlayerStatus_Origin(FPlayerTotalData NewPlayerData) {MyPlayerTotalStatus_Origin = NewPlayerData; UpdateStatus();}; // MyPlayerTotalStatus 설정 
	FORCEINLINE FPlayerTotalData GetPlayerStatus() const {return MyPlayerTotalStatus;}; // MyPlayerTotalStatus 반환
	FORCEINLINE void SetPlayerStatus(FPlayerTotalData NewPlayerData) {MyPlayerTotalStatus = NewPlayerData; UpdateStatus();}; // MyPlayerTotalStatus 설정 

	FORCEINLINE float GetDefaultMaxHP_Origin() const {return MyPlayerTotalStatus_Origin.PlayerDynamicData.MaxHP;}; // DefaultSpeed 반환;
	FORCEINLINE float GetDefaultHP_Origin() const {return MyPlayerTotalStatus_Origin.PlayerDynamicData.HP;}; // DefaultSpeed 반환;
	FORCEINLINE float GetDefaultSpeed_Origin() const {return MyPlayerTotalStatus_Origin.PlayerDynamicData.MoveSpeed;}; // DefaultSpeed 반환;
	FORCEINLINE float GetDefaultATK_Origin() const {return MyPlayerTotalStatus_Origin.PlayerDynamicData.ATK;};

	FORCEINLINE float GetDefaultMaxHP() const {return MyPlayerTotalStatus.PlayerDynamicData.MaxHP;}; // DefaultSpeed 반환;
	FORCEINLINE float GetDefaultHP() const {return MyPlayerTotalStatus.PlayerDynamicData.HP;}; // DefaultSpeed 반환;
	FORCEINLINE float GetDefaultATK()  const {return MyPlayerTotalStatus.PlayerDynamicData.ATK;}; // DefaultATK 반환
	FORCEINLINE int32 GetDefaultMP()  const {return MyPlayerTotalStatus.PlayerDynamicData.MP;}; // DefaultATK 반환
	FORCEINLINE int32 GetDefaultMaxMP()  const {return MyPlayerTotalStatus.PlayerDynamicData.MaxMP;}; // DefaultATK 반환
	virtual float GetCurrentATK() const {return MyPlayerTotalStatus.PlayerDynamicData.ATK;};

	FORCEINLINE void SetDefaultATK(float NewValue) {MyPlayerTotalStatus.PlayerDynamicData.ATK = NewValue; UpdateStatus();}; // DefaultATK 설정
	FORCEINLINE void SetDefaultSpeed(float Speed) {MyPlayerTotalStatus.PlayerDynamicData.MoveSpeed = Speed; UpdateStatus();}; // DefaultSpeed 설정;
	FORCEINLINE void SetDefaultATKSpeed(float NewValue) {MyPlayerTotalStatus.PlayerDynamicData.ATKSpeed = NewValue; UpdateStatus();}; // DefaultATKSpeed 설정
	FORCEINLINE void SetDefaultHP(float NewValue) {MyPlayerTotalStatus.PlayerDynamicData.HP = NewValue; UpdateStatus();}; // DefaultHP 설정
	FORCEINLINE void SetDefaultMaxHP(float NewValue) {MyPlayerTotalStatus.PlayerDynamicData.MaxHP = NewValue; UpdateStatus();}; // DefaultMaxHP 설정
	FORCEINLINE void SetDefaultMP(uint8 NewValue) {MyPlayerTotalStatus.PlayerDynamicData.MP = NewValue; UpdateStatus();}; // DefaultMP 설정
	FORCEINLINE void SetDefaultCriticalPercent(float NewValue) {MyPlayerTotalStatus.PlayerDynamicData.CriticalPercent = NewValue; UpdateStatus();}; // CriticalPercent 설정
	FORCEINLINE void SetDefaultCriticalDamage(float NewValue) {MyPlayerTotalStatus.PlayerDynamicData.CriticalDamageCoefficient = NewValue; UpdateStatus();}; // CriticalDamageCoefficient 설정
	FORCEINLINE void SetDefaultDEF(float NewValue) {MyPlayerTotalStatus.PlayerDynamicData.DEF = NewValue; UpdateStatus();}; // DefaultDEF 설정
	// FORCEINLINE void SetDefaultMaxMP(uint8 NewValue) {MyPlayerTotalStatus.PlayerDynamicData.MaxMP = NewValue; UpdateStatus();}; // DefaultMaxMP 설정

	FORCEINLINE void SetDefaultATK_Origin(float NewValue) {MyPlayerTotalStatus_Origin.PlayerDynamicData.ATK = NewValue; OnUpdateOriginStatus.Broadcast();}; // DefaultATK 설정
	FORCEINLINE void SetDefaultSpeed_Origin(float Speed) {MyPlayerTotalStatus_Origin.PlayerDynamicData.MoveSpeed = Speed; OnUpdateOriginStatus.Broadcast();}; // DefaultSpeed 설정;
	FORCEINLINE void SetDefaultATKSpeed_Origin(float NewValue) {MyPlayerTotalStatus_Origin.PlayerDynamicData.ATKSpeed = NewValue; OnUpdateOriginStatus.Broadcast();}; // DefaultATKSpeed 설정
	FORCEINLINE void SetDefaultHP_Origin(float NewValue) {MyPlayerTotalStatus_Origin.PlayerDynamicData.HP = NewValue; OnUpdateOriginStatus.Broadcast();}; // DefaultHP 설정
	FORCEINLINE void SetDefaultMaxHP_Origin(float NewValue) {MyPlayerTotalStatus_Origin.PlayerDynamicData.MaxHP = NewValue; OnUpdateOriginStatus.Broadcast();}; // DefaultMaxHP 설정
	FORCEINLINE void SetDefaultMP_Origin(uint8 NewValue) {MyPlayerTotalStatus_Origin.PlayerDynamicData.MP = NewValue; OnUpdateOriginStatus.Broadcast();}; // DefaultMP 설정
	FORCEINLINE void SetDefaultMaxMP_Origin(uint8 NewValue) {MyPlayerTotalStatus_Origin.PlayerDynamicData.MaxMP = NewValue;  UpdateStatus();}; // DefaultMaxMP 설정
	FORCEINLINE void SetDefaultCriticalPercent_Origin(float NewValue) {MyPlayerTotalStatus_Origin.PlayerDynamicData.CriticalPercent = NewValue; OnUpdateOriginStatus.Broadcast();}; // DefaultCriticalPercent 설정
	FORCEINLINE void SetDefaultCriticalDamage_Origin(float NewValue) {MyPlayerTotalStatus_Origin.PlayerDynamicData.CriticalDamageCoefficient = NewValue; OnUpdateOriginStatus.Broadcast();}; // DefaultCriticalDamageCoefficient 설정
	FORCEINLINE void SetDefaultDEF_Origin(float NewValue) {MyPlayerTotalStatus_Origin.PlayerDynamicData.DEF = NewValue; OnUpdateOriginStatus.Broadcast();}; // DefaultDEF 설정

	virtual void UpdateStatus();

	FORCEINLINE bool IsCriticalAttack() const {return bCriticalAttack;};
	// 크리티컬 데미지 계산
	float CriticalCalculate();

	// Move 관련 함수
	FORCEINLINE bool GetCanMove() const {return bCanMove;}; // bCanMove 반환;
	FORCEINLINE void SetCanMove(bool NewValue) {bCanMove = NewValue;}; // bCanMove 설정;
	FORCEINLINE void SetCanJog(bool NewValue) {bCanJog = NewValue;}; // bCanJog 설정;

	// Attack 관련 함수
	FORCEINLINE UAPAttackComponent* GetAttackComponent() const {return AttackComponent;};  // AttackComp 반환
	virtual void SetAttackRotation();

protected:
	//Component
	UPROPERTY(EditAnywhere, Category = "Component")
	UAPCrowdControlComponent* CrowdControlComp;
	
	UPROPERTY(EditAnywhere, Category = "Component")
	UAPMovementComponent* MoveComponent;

	UPROPERTY(EditAnywhere, Category = "Component")
	UAPAttackComponent* AttackComponent;

	UPROPERTY(EditAnywhere, Category = "Component")
	UAPHitPointComponent* HitPointComponent;
	
	UPROPERTY(EditAnywhere, Category = "Component")
	UAPAnimHubComponent* AnimHubComponent;

	UPROPERTY(EditAnywhere, Category = "CC State")
	UNiagaraComponent* StunEffectComp;

	UPROPERTY(EditAnywhere, Category = "CC State")
	UNiagaraComponent* FrozenEffectComp;

	// 무적기 관련 변수
	FTimerHandle BlockTimerHandle;
	UPROPERTY(EditAnywhere, Category = "Super Stance")
	float BlockTime = 1.5f;
	bool bBlockMode = false;

	// 머터리얼
	UMaterialInterface* DefaultMaterial;

	// 캐릭터 상태 관련 변수
	bool bHitting = false;
	ECharacterState CurrentState = ECharacterState::None;
	float DefaultSlip = 0.0f;
	bool bDoing = false; // 공격 , 스킬 사용중인지 체크하는 변수 / true면 다른 행동 제약 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FPlayerTotalData MyPlayerTotalStatus_Origin; // 오리지널 스테이터스 (영구적 증가)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FPlayerTotalData MyPlayerTotalStatus; // 현재 스테이터스 (일시적 증가) [ex - 장비, 버프 등] // 플레이아 캐릭터만 업데이트해서 사용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FGameData MyGameStatus;
	
	bool bCriticalAttack = false;

	// 이동 관련 변수
	bool bCanMove = true;
	bool bCanJog = true;

public:
	TArray<bool> StopState;

	FOnCrowdControlCheck OnCrowdControlCheck;

	FOnSkillTrigger OnSkillTrigger;

	FOnSkillEndTrigger OnSkillEndTrigger;

	FOnSkillEndWithSkillKey OnSkillEndWithSkillKey;

	FOnSkillChargingTrigger OnSkillChargingTrigger;

	FOnSkillEnhanceTrigger OnSkillEnhanceTrigger;

	FOnMaxHPAndHPChanged OnMaxHPAndHPChanged;

	FOnSkillRotationTrigger OnSkillRotationTrigger;

	FOnUpdateOriginStatus OnUpdateOriginStatus;
};
