// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/APCharacterBase.h"
#include "Enemy_CharacterBase.generated.h"

class ATextRenderActor;
class UAP_EnemyBaseAnimInstance;
class UNiagaraComponent;
class AEnemy_DropBase;
class AEnemyBaseAIController;
class AEnemy_DropPackage;
class UNiagaraSystem;
class UAPMovementComponent;
class USkillNumberBase;
class UAPManaDropComponent;
class AAPEnemyAttackRange;
class AAPHUD;
class UWidgetComponent;
class UAPEnemyTextWidgetComponent;
class UAPEnemyHPWidgetComponent;

UENUM(BlueprintType)
enum class EBossPhase : uint8 // E스킬
{
	None 			 = 0 UMETA(DisplayName = "None"),
	Phase_1			 = 1 UMETA(DisplayName = "Phase_1"),
	Phase_2			 = 2 UMETA(DisplayName = "Phase_2"),
	Phase_3			 = 3 UMETA(DisplayName = "Phase_3"),
};

UCLASS()
class ARCANEPUNK_API AEnemy_CharacterBase : public AAPCharacterBase
{
	GENERATED_BODY()

public:
	AEnemy_CharacterBase();
	// 드롭 데이타
	FORCEINLINE FDropData GetDropData() const {return DropData;};

	// 데미지 계수
	FORCEINLINE float GetDamageMultiple() const {return DamageMultiple;};

	// 공격 범위 관련
	FORCEINLINE float GetAttackRangeTime()const {return AttackRangeTime;}; 
	FORCEINLINE float GetMonsterAttackRange()const {return Monster_AttackRange;}; 

	// 탐지 범위
	FORCEINLINE float GetDetectDist()const {return DetectDist;}; 

	// 흡혈 영역
	FORCEINLINE bool IsInDrainField() const {return InDrainField;}; // InDrainField 반환;
	FORCEINLINE void AddInDrainField(bool NewBool) { InDrainField = NewBool;};

	// 패링 경직 여부
	FORCEINLINE bool CanParryingCounter() const {return bCanParryingCounter;};

protected:
	virtual void BeginPlay() override;

	virtual void DissolveCharacterMesh(float DeltaTime) override;
	void OnClearGodMode();

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void OnHittingEnd() override;

	void SetOwnerSpawnVolume(AActor* SpawnVolume);

	void AddDamageMultiple(float Add, float Time);
	void OnAttachingDamagedMark(float Time);

	// 몬스터 Attack 관련 함수
	virtual void PossessedBy(AController* NewController) override;
	virtual void NormalAttack();

	// void TeleportMarkActivate(float Time, AActor * MarkOwner, USkillNumberBase* SkillComp);
	// void TeleportMarkDeactivate();

	bool AttackPushBack(AActor* DamagedCauser);

	void RotateTowardsTarget(AActor *TargetActor, float Speed = -1.0f);

	virtual void SpawnAttackRange(AActor* Target, float WaitTime);

	// HitPoint 관련 함수
	void DistinctHitPoint(const FVector & ImpactPoint, AActor* HitActor);

	// 몬스터 Anim 관련 함수
	UFUNCTION()
	void EnemyMontageEnded(UAnimMontage *Montage, bool bInterrupted);

	// 몬스터 CrowdControl 관련 함수
	UFUNCTION(BlueprintPure)
	bool IsHardCC();

	// 몬스터 어그로
	AActor* IsAggro();

	// 패트롤 위치
	FVector GetPatrolLocation(const FVector & Start);

	// Detect Render 스폰
	void SpawnDetectRender();

	// HP UI
	UFUNCTION(BlueprintPure)
	virtual bool SetHPUI();
	virtual void RemoveHPUI();

	// Enemy Rescue
	virtual void RescueEnemy();

protected:
	bool AttackTrace(FHitResult &HitResult, FVector &HitVector, float Radius, const FVector & Start, const FVector & End);
	void SpawnDamageText(AController* EventInstigator, float Damage, const FVector & AddLocation);

	//Monster 세팅 초기화
	void InitMonster();
	void StopClear();

	//드롭 관련 함수
	void SetManaDrop();
	void DropChecking(AActor *DamageCauser);

	// 몬스터 Dead 관련 함수
	virtual void EnemyDeadMotion();
	virtual void EnemyDestroyed();
	void CheckAllEnemyKilled();
	
	// 몬스터 Anim Ended 관련 함수
	void BindMontageEnded();

	UFUNCTION()
	void OnNormalAttack_MontageEnded();
	UFUNCTION()
	virtual void OnDeath_MontageEnded();
	UFUNCTION()
	void OnDetect_MontageEnded();
	UFUNCTION()
	void OnHit_MontageEnded();

	// 몬스터 CC기 관련 함수
	void OnPlayerKnockBack(AActor* Actor, float Dist, float Time);
	void OnPlayerStun(AActor *Actor, float Time);
	void CrowdControlCheck();


	// 피격 데미지 배수 관련 함수
	void SubtractDamageMultiple(float Subtract);
	void OnDetachingDamagedMark();

	float GetTargetDist2D(AActor* Target);
	void SetCapsuleOverlap(bool IsOverlap);
	
protected:
	UPROPERTY(EditAnywhere, Category = "Status")
	FDropData DropData;

	UPROPERTY(EditAnywhere, Category = "Status")
	UAPManaDropComponent* ManaDropComp;

	UPROPERTY(EditAnywhere, Category = "Widget")
	UWidgetComponent* TextWidgetComp;

	UPROPERTY(EditAnywhere, Category = "Widget")
	UWidgetComponent* HealthWidgetComp;

	TWeakObjectPtr<UAPEnemyTextWidgetComponent> TextUI;

	TWeakObjectPtr<UAPEnemyHPWidgetComponent> HPUI;

	UPROPERTY(EditAnywhere, Category = "Status")
	float Monster_AttackRange = 250.0f; // if equal, Trace Stop

	UPROPERTY(EditAnywhere, Category = "Status")
	float Monster_AttackRadius = 80.0f;

	UPROPERTY(EditAnywhere, Category = "Status")
	float Monster_AttackRange_Plus = 100.0f; // AttackRange_Plus = AttackRange + AttackRange_Plus;

	UPROPERTY(EditAnywhere, Category = "Status") 
	float DetectDist = 150.0f; // Trace Dist

	// UPROPERTY(EditAnywhere)
	// UNiagaraComponent* TeleportMark;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* DamagedMark;

	bool bKnockBackAttack = false;

	// TimerHandle
		// FTimerHandle TeleportTimerHandle;
	FTimerHandle PresentDamageTimerHandle;
	FTimerHandle StopTimerHandle;
	FTimerHandle DamageMultipleTimerHandle;
	FTimerHandle DamagedMarkTimerHandle;
	// 머터리얼

	// 드롭 관련 변수
	// UPROPERTY(EditAnywhere, Category = "Drop")
	// TMap<TSubclassOf<AEnemy_DropBase>, float> DropMap; // Drop Class , Drop 확률 (따로 드랍)

	TWeakObjectPtr<AEnemy_DropPackage> DropPackage;

	TWeakObjectPtr<AActor> MarkActor;

	// 몬스터 Hit 관련 변수
	UPROPERTY(EditAnywhere, Category = "Hit")
	bool IsAttackPush = true;

	UPROPERTY(EditAnywhere, Category = "Hit")
	FVector DamageTextAddLocation = FVector(0,0,0);
	UPROPERTY(EditAnywhere, Category = "Hit")
	float HitPushDist = 60.0f;

	// 몬스터 Anim 관련 변수
	TWeakObjectPtr<UAP_EnemyBaseAnimInstance> EnemyAnim;

	UPROPERTY(EditAnywhere, Category = "Move")
	TEnumAsByte<enum EMovementMode> Basic_MOVE = EMovementMode::MOVE_Walking;

	// 몬스터 CC기 관련 변수
	UPROPERTY(EditAnywhere, Category = "CC")
	float KnockBackTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "CC")
	float StunTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "KnockBackAttack")
	float KnockBackDist = 1500.0f;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* Weapon;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> DamageTextClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> DetectTextClass;

	float DamageMultiple = 1.0f;

	bool InDrainField = false;

	TWeakObjectPtr<USkillNumberBase> SkillComponent;

	UPROPERTY(EditAnywhere)
	USkeletalMesh* DefaultWeapon;

	UPROPERTY(EditAnywhere)
	float PatrolDist = 500.0f;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AAPEnemyAttackRange> AttackRangeClass;
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AAPEnemyAttackRange> AttackRangeClass_2;
	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackRangeTime = 1.2f;

	//AAPHUD
	TWeakObjectPtr<AAPHUD> PlayerHUD;

	UPROPERTY(EditAnywhere, Category = "Parrying")
	bool bCanParryingCounter = true;

	UPROPERTY(EditAnywhere)
	bool bNormalMonster = true;

	bool bEnemyDeath = false;

	bool bRescue = false;

	TWeakObjectPtr<AActor> OwnerSpawnVolume;
};
