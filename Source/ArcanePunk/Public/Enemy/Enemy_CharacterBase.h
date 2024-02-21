// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/Common/APCrowdControlComponent.h"
#include "Enemy_CharacterBase.generated.h"

#define Defense_constant 1000

DECLARE_MULTICAST_DELEGATE(FOnHPChanged);
DECLARE_MULTICAST_DELEGATE(FOnCrowdControlCheck);

class ATextRenderActor;
class UAP_EnemyBaseAnimInstance;
class UNiagaraComponent;
class AEnemy_DropBase;
class UWidgetComponent;
class AEnemyBaseAIController;
class AEnemy_DropPackage;

UCLASS()
class ARCANEPUNK_API AEnemy_CharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy_CharacterBase();

protected:
	virtual void BeginPlay() override;

	TArray<ATextRenderActor*> PresentDamages;
public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	// 몬스터 Dead , Hit 관련 함수
	UFUNCTION(BlueprintPure)
	virtual bool IsDead();

	UFUNCTION(BlueprintPure)
	bool IsHitting();

	FORCEINLINE void SetHitting(bool NewBool) {bHitting = NewBool;};

	UFUNCTION(BlueprintPure)
	float GetForward();

	UFUNCTION(BlueprintPure)
	float GetRight();

	void SetHitPoint(float Forward, float Right);

	// 몬스터 Attack 관련 함수
	virtual void PossessedBy(AController* NewController) override;
	void NormalAttack();

	float GetDistanceLimit();

	void TeleportMarkActivate(float Time, AActor * MarkOwner);
	void TeleportMarkDeactivate();

	bool AttackPushBack(FVector NewLocation);

	FORCEINLINE float GetMonsterATK() const { return Monster_ATK;}; //Monster_ATK 반환

	// HitPoint 관련 함수
	void DistinctHitPoint(FVector ImpactPoint, AActor* HitActor);

	// 몬스터 Anim 관련 함수
	UFUNCTION()
	void EnemyMontageEnded(UAnimMontage *Montage, bool bInterrupted);

	// 몬스터 HP 관련 함수
	FORCEINLINE float GetMonsterMaxHP() const { return MaxHP;}; //Monster_MaxHP 반환
	FORCEINLINE float GetMonsterHP() const { return HP;}; //Monster_HP 반환

	// 몬스터 CrowdControl 관련 함수
	FORCEINLINE UAPCrowdControlComponent* GetCrowdControlComp() const { return CrowdControlComp;};
	UFUNCTION(BlueprintPure)
	FORCEINLINE ECharacterState ReturnState() const { return CurrentState;};
	FORCEINLINE void SetState(ECharacterState UpdateState) { CurrentState = UpdateState;};
	FORCEINLINE UNiagaraComponent* GetStunEffect() const { return StunEffect;};
	UFUNCTION(BlueprintPure)
	bool IsHardCC();

	// 몬스터 어그로
	AActor* IsAggro();

protected:
	float DamageMath(float Damage);
	bool AttackTrace(FHitResult &HitResult, FVector &HitVector, bool Custom = false, float Radius = 0.0f, FVector CustomStart = FVector(0,0,0), FVector CustomEnd = FVector(0,0,0));
	void ResetHitStiffness();
	void SpawnDamageText(float Damage, FVector AddLocation);

	// Critical Damage Calculate
	float CriticalCalculate(float Multiple = 2);

	//HP 세팅 초기화
	void InitHP();

	//드롭 관련 함수
	void DropItemActor();

	// 몬스터 Dead 관련 함수
	virtual void EnemyDestroyed();
	void CheckAllEnemyKilled();
	
	// HitPoint 관련 함수
	void TestHit();

	// 몬스터 Anim Ended 관련 함수
	void BindMontageEnded();

	void OnNormalAttack_MontageEnded();

	// 몬스터 CC기 관련 함수
	void OnPlayerKnockBack(AActor* Actor, float Dist, float Time);
	void OnPlayerStun(AActor *Actor, float Time);
	void CrowdControlCheck();

protected:
	//Component
	UPROPERTY(EditAnywhere, Category = "Component")
	UAPCrowdControlComponent* CrowdControlComp;

	// 몬스터 Status 관련 변수
	UPROPERTY(EditAnywhere, Category = "Status")
	float MaxHP = 100.0f;

	UPROPERTY()
	float HP = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Status")
	UWidgetComponent* HealthWidgetComp;

	UPROPERTY(EditAnywhere, Category = "Status")
	float Character_Defense = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Status")
	float Monster_AttackRange = 250.0f;

	UPROPERTY(EditAnywhere, Category = "Status")
	float Monster_AttackRadius = 80.0f;

	UPROPERTY(EditAnywhere, Category = "Status")
	float Monster_ATK = 25.0f;

	UPROPERTY(EditAnywhere, Category = "Status")
	float Distance_Limit = 800.0f;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* TeleportMark;

	UPROPERTY(EditAnywhere)
	float CriticalPercent = 5.0f;

	UPROPERTY(EditAnywhere)
	int32 CriticalStep = 1;

	UPROPERTY(EditAnywhere)
	int32 NormalAttack_CriticalMultiple = 2;

	UPROPERTY()
	bool bKnockBackAttack = false;

	// TimerHandle
	FTimerHandle PresentDamageTimerHandle;
	FTimerHandle TeleportTimerHandle;
	FTimerHandle HitTimerHandle;
	FTimerHandle DeathTimerHandle;

	// 머터리얼
	UMaterialInterface* DefaultMaterial;

	UPROPERTY(EditAnywhere, Category = "Skin")
	UMaterialInterface* HitMaterial; // 피격 점멸 머터리얼

	UPROPERTY(EditAnywhere, Category = "Skin")
	UMaterialInterface* HitMaterial_Test1; // 피격 점멸 머터리얼

	UPROPERTY(EditAnywhere, Category = "Skin")
	UMaterialInterface* HitMaterial_Test2; // 피격 점멸 머터리얼

	float DefaultSlip;

	// 드롭 관련 변수
	UPROPERTY(EditAnywhere, Category = "Drop")
	TMap<TSubclassOf<AEnemy_DropBase>, float> DropMap; // Drop Class , Drop 확률 (따로 드랍)

	UPROPERTY(EditAnywhere, Category = "Drop")
	TMap<FName , float> PackageDropMap; // Item ID , Drop 확률 (보따리 드랍)

	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemy_DropPackage> DropPackageClass;

	UPROPERTY()
	AEnemy_DropPackage* DropPackage;

	bool OnDrop = false;

	// 몬스터 Dead 관련 변수
	UPROPERTY(EditAnywhere, Category = "Dead")
	float DeathLoadingTime = 3.0f;

	bool bIsDead = false;

	AActor* MarkActor;

	// 몬스터 Hit 관련 변수
	UPROPERTY()
	bool bHitting = false;

	UPROPERTY(EditAnywhere, Category = "Hit")
	float HitStiffnessTime = 0.75f;

	UPROPERTY(EditAnywhere, Category = "Hit")
	bool IsAttackPush = true;

	float MonsterIsForward = 0.0f;
	float MonsterIsRight = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Hit")
	FVector DamageTextAddLocation = FVector(0,0,0);

	// 몬스터 Anim 관련 변수
	UPROPERTY()
	UAP_EnemyBaseAnimInstance* EnemyAnim;

	// 몬스터 CC기 관련 변수
	UPROPERTY(EditAnywhere, Category = "CC")
	float KnockBackTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "CC")
	float StunTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "KnockBackAttack")
	float KnockBackDist = 1500.0f;
	
	UPROPERTY(EditAnywhere, Category = "CC")
	UNiagaraComponent* StunEffect;
	
	ECharacterState CurrentState = ECharacterState::None;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* Weapon;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> DamageTextClass;

public:
	FOnHPChanged OnEnemyHPChanged;

	FOnCrowdControlCheck OnCrowdControlCheck;

	UPROPERTY()
	TArray<bool> StopState;
};
