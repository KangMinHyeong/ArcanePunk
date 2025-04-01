// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameState/APGameState.h"
#include "PlayerState/APPlayerData.h"
#include "Character/SkillDataTable/SkillDataTable.h"
#include "Components/Common/APCrowdControlComponent.h"
#include "GameFramework/Character.h"
#include "APCharacterBase.generated.h"

#define Defense_constant 1000

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateOriginStatus);
DECLARE_MULTICAST_DELEGATE(FOnCrowdControlCheck);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillTrigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillEndTrigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillEndWithSkillKey, ESkillKey, SkillKey, ESkillNumber, SkillNumber);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillChargingTrigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillChargingEndTrigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillEnhanceTrigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillRotationTrigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMaxHPAndHPChanged, float, MaxHP, float, HP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCheckingShield, AActor*, Owner, float, OriginShieldHP);

class UAPMovementComponent;
class UNiagaraComponent;
class UAPAttackComponent;
class UAPHitPointComponent;
class UAPAnimHubComponent;
class UNiagaraSystem;

UCLASS()
class ARCANEPUNK_API AAPCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AAPCharacterBase();
	virtual void PostInitializeComponents() override; 

protected:
	virtual void BeginPlay() override;

public:	
	FORCEINLINE UAPMovementComponent* GetAPMoveComponent() const {return MoveComponent;}; // MoveComp 반환
	FORCEINLINE UAPCrowdControlComponent* GetCrowdControlComp() const { return CrowdControlComp;};

	FORCEINLINE TArray<UMaterialInterface*> GetDefaultMaterial() const {return DefaultMaterial;}; // DefaultMaterial 반환

	FORCEINLINE UAPHitPointComponent* GetHitPointComponent() const {return HitPointComponent;}; // HitPointComp 반환

	// 머터리얼 관련 함수
	void ResetDefaultMaterial();
	
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
	FORCEINLINE FPlayerTotalData GetPlayerStatus_Origin() const {return TotalStatus_Origin;}; // MyPlayerTotalStatus 반환
	FORCEINLINE void SetPlayerStatus_Origin(const FPlayerTotalData & NewPlayerData) {TotalStatus_Origin = NewPlayerData; UpdateStatus();}; // MyPlayerTotalStatus 설정 
	FORCEINLINE FPlayerTotalData GetPlayerStatus() const {return TotalStatus;}; // MyPlayerTotalStatus 반환
	FORCEINLINE void SetPlayerStatus(const FPlayerTotalData & NewPlayerData) {TotalStatus = NewPlayerData; UpdateStatus();}; // MyPlayerTotalStatus 설정 

	FORCEINLINE float GetDefaultMaxHP_Origin() const {return TotalStatus_Origin.StatusData.MaxHP;}; // DefaultSpeed 반환;
	FORCEINLINE float GetDefaultHP_Origin() const {return TotalStatus_Origin.StatusData.HP;}; // DefaultSpeed 반환;
	FORCEINLINE float GetDefaultSpeed_Origin() const {return TotalStatus_Origin.StatusData.MoveSpeed;}; // DefaultSpeed 반환;
	FORCEINLINE float GetDefaultATK_Origin() const {return TotalStatus_Origin.StatusData.ATK;};

	FORCEINLINE float GetDefaultMaxHP() const {return TotalStatus.StatusData.MaxHP;}; // DefaultSpeed 반환;
	FORCEINLINE float GetDefaultHP() const {return TotalStatus.StatusData.HP;}; // DefaultSpeed 반환;
	FORCEINLINE float GetDefaultATK()  const {return TotalStatus.StatusData.ATK;}; // DefaultATK 반환
	FORCEINLINE int32 GetDefaultMP()  const {return TotalStatus.StatusData.MP;}; // DefaultATK 반환
	FORCEINLINE int32 GetDefaultMaxMP()  const {return TotalStatus.StatusData.MaxMP;}; // DefaultATK 반환
	virtual float GetCurrentATK() const {return TotalStatus.StatusData.ATK;};

	FORCEINLINE void SetDefaultATK(float NewValue) {TotalStatus.StatusData.ATK = NewValue; UpdateStatus();}; // DefaultATK 설정
	FORCEINLINE void SetDefaultSpeed(float Speed) {TotalStatus.StatusData.MoveSpeed = Speed; UpdateStatus();}; // DefaultSpeed 설정;
	FORCEINLINE void SetDefaultATKSpeed(float NewValue) {TotalStatus.StatusData.ATKSpeed = NewValue; UpdateStatus();}; // DefaultATKSpeed 설정
	FORCEINLINE void SetDefaultHP(float NewValue) {TotalStatus.StatusData.HP = NewValue; UpdateStatus();}; // DefaultHP 설정
	FORCEINLINE void SetDefaultMaxHP(float NewValue) {TotalStatus.StatusData.MaxHP = NewValue; UpdateStatus();}; // DefaultMaxHP 설정
	FORCEINLINE void SetDefaultMP(uint8 NewValue) {TotalStatus.StatusData.MP = NewValue; UpdateStatus();}; // DefaultMP 설정
	FORCEINLINE void SetDefaultCriticalPercent(float NewValue) {TotalStatus.StatusData.CriticalPercent = NewValue; UpdateStatus();}; // CriticalPercent 설정
	FORCEINLINE void SetDefaultCriticalDamage(float NewValue) {TotalStatus.StatusData.CriticalDamageCoefficient = NewValue; UpdateStatus();}; // CriticalDamageCoefficient 설정
	FORCEINLINE void SetDefaultDEF(float NewValue) {TotalStatus.StatusData.DEF = NewValue; UpdateStatus();}; // DefaultDEF 설정
	// FORCEINLINE void SetDefaultMaxMP(uint8 NewValue) {MyPlayerTotalStatus.PlayerDynamicData.MaxMP = NewValue; UpdateStatus();}; // DefaultMaxMP 설정

	FORCEINLINE void SetDefaultATK_Origin(float NewValue) {TotalStatus_Origin.StatusData.ATK = NewValue; OnUpdateOriginStatus.Broadcast();}; // DefaultATK 설정
	FORCEINLINE void SetDefaultSpeed_Origin(float Speed) {TotalStatus_Origin.StatusData.MoveSpeed = Speed; OnUpdateOriginStatus.Broadcast();}; // DefaultSpeed 설정;
	FORCEINLINE void SetDefaultATKSpeed_Origin(float NewValue) {TotalStatus_Origin.StatusData.ATKSpeed = NewValue; OnUpdateOriginStatus.Broadcast();}; // DefaultATKSpeed 설정
	FORCEINLINE void SetDefaultHP_Origin(float NewValue) {TotalStatus_Origin.StatusData.HP = NewValue; OnUpdateOriginStatus.Broadcast();}; // DefaultHP 설정
	FORCEINLINE void SetDefaultMaxHP_Origin(float NewValue) {TotalStatus_Origin.StatusData.MaxHP = NewValue; OnUpdateOriginStatus.Broadcast();}; // DefaultMaxHP 설정
	FORCEINLINE void SetDefaultMP_Origin(uint8 NewValue) {TotalStatus_Origin.StatusData.MP = NewValue; OnUpdateOriginStatus.Broadcast();}; // DefaultMP 설정
	FORCEINLINE void SetDefaultMaxMP_Origin(uint8 NewValue) {TotalStatus_Origin.StatusData.MaxMP = NewValue;  UpdateStatus();}; // DefaultMaxMP 설정
	FORCEINLINE void SetDefaultCriticalPercent_Origin(float NewValue) {TotalStatus_Origin.StatusData.CriticalPercent = NewValue; OnUpdateOriginStatus.Broadcast();}; // DefaultCriticalPercent 설정
	FORCEINLINE void SetDefaultCriticalDamage_Origin(float NewValue) {TotalStatus_Origin.StatusData.CriticalDamageCoefficient = NewValue; OnUpdateOriginStatus.Broadcast();}; // DefaultCriticalDamageCoefficient 설정
	FORCEINLINE void SetDefaultDEF_Origin(float NewValue) {TotalStatus_Origin.StatusData.DEF = NewValue; OnUpdateOriginStatus.Broadcast();}; // DefaultDEF 설정

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
	virtual void SetAttackRotation(float AddSpeed = 0.0f);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	virtual void OnHittingEnd();
	bool CheckShieldHP(float & DamageApplied, FDamageEvent const &DamageEvent);

	void SpawnVoiceSound(USoundBase* VoiceSound);
	void SpawnAttackVoiceSound();
	void SpawnAttackSound(uint8 AttackNum);

	void SetHitPoint(float Forward, float Right);
	void SetHitEffect(FVector HitLocation);
	
	virtual void UpdateSwapGauge(float Add = 10.0f) {};

protected:
	float DamageMath(float Damage);
	virtual void DissolveCharacterMesh(float DeltaTime);

protected:
	UPROPERTY(EditAnywhere)
	FName CharacterName = "";

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
	UPROPERTY(EditAnywhere, Category = "Super Stance")
	bool bBlockMode = false;

	// 머터리얼
	UPROPERTY()
	TArray<UMaterialInterface*> DefaultMaterial;

	UPROPERTY(EditAnywhere, Category = "Hit")
	UMaterialInterface* HitMaterial;

	// 캐릭터 상태 관련 변수
	ECharacterState CurrentState = ECharacterState::None;
	float DefaultSlip = 0.0f;
	bool bDoing = false; // 공격 , 스킬 사용중인지 체크하는 변수 / true면 다른 행동 제약 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FPlayerTotalData TotalStatus_Origin; // 오리지널 스테이터스 (영구적 증가)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FPlayerTotalData TotalStatus; // 현재 스테이터스 (일시적 증가) [ex - 장비, 버프 등] // 플레이아 캐릭터만 업데이트해서 사용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FGameData MyGameStatus;
	
	bool bCriticalAttack = false;

	// 이동 관련 변수
	bool bCanMove = true;
	bool bCanJog = true;

	// Hit 관련 변수
	bool bHitting = false;
	FTimerHandle HitTimerHandle;
	FTimerHandle HitMaterialTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Hit")
	float HitMotionTime = 0.6f;
	UPROPERTY(EditAnywhere, Category = "Hit")
	float HitMaterailTime = 0.4f;
	UPROPERTY(EditAnywhere, Category = "Hit")
	USoundBase* HitVoiceSound;
	UPROPERTY(EditAnywhere, Category = "Hit")
	UNiagaraSystem* HitEffect_L;
	UPROPERTY(EditAnywhere, Category = "Hit")
	UNiagaraSystem* HitEffect_R;
	UPROPERTY(EditAnywhere, Category = "Hit")
	UNiagaraSystem* HitEffect_B;

	UPROPERTY(EditAnywhere, Category = "Hit")
	UNiagaraSystem* ShieldHitEffect; 

	float HitForward = 0.0f;
	float HitRight = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Dead")
	USoundBase* DeadVoiceSound;

	UPROPERTY(EditAnywhere, Category = "Attack")
	USoundBase* AttackVoiceSound;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TArray<USoundBase*> AttackSound;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> SkinMesh;
	float Apperence = 1.0f;
	UPROPERTY(EditAnywhere)
	float FadeOutSpeed = 0.275f;

public:
	TArray<bool> StopState;

	FOnCrowdControlCheck OnCrowdControlCheck;

	FOnSkillTrigger OnSkillTrigger;

	FOnSkillEndTrigger OnSkillEndTrigger;

	FOnSkillEndWithSkillKey OnSkillEndWithSkillKey;

	FOnSkillChargingTrigger OnSkillChargingTrigger;

	FOnSkillChargingEndTrigger OnSkillChargingEndTrigger;

	FOnSkillEnhanceTrigger OnSkillEnhanceTrigger;

	FOnMaxHPAndHPChanged OnMaxHPAndHPChanged;

	FOnSkillRotationTrigger OnSkillRotationTrigger;

	FOnUpdateOriginStatus OnUpdateOriginStatus;
 
	FOnCheckingShield OnCheckingShield;


	// Test 디버그 드로우
	UPROPERTY(EditAnywhere)
	bool bDebugDraw = true;
};
