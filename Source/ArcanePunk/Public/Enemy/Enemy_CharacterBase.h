// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/APCharacterBase.h"
#include "Enemy_CharacterBase.generated.h"

#define Defense_constant 1000

DECLARE_MULTICAST_DELEGATE(FOnHPChanged);

class ATextRenderActor;
class UAP_EnemyBaseAnimInstance;
class UNiagaraComponent;
class AEnemy_DropBase;
class UWidgetComponent;
class AEnemyBaseAIController;
class AEnemy_DropPackage;
class UNiagaraSystem;
class UAPMovementComponent;
class USkillNumberBase;

UCLASS()
class ARCANEPUNK_API AEnemy_CharacterBase : public AAPCharacterBase
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

	UFUNCTION(BlueprintPure)
	float GetForward();

	UFUNCTION(BlueprintPure)
	float GetRight();

	void SetHitPoint(float Forward, float Right);

	FORCEINLINE float GetDamageMultiple() const {return DamageMultiple;};
	void AddDamageMultiple(float Add, float Time);
	void OnAttachingDamagedMark(float Time);

	// 몬스터 Attack 관련 함수
	virtual void PossessedBy(AController* NewController) override;
	void NormalAttack();

	FORCEINLINE float GetDistanceLimit()const {return Distance_Limit;}; 
	FORCEINLINE float GetDetectLimit()const {return DetectLimit;}; 

	void TeleportMarkActivate(float Time, AActor * MarkOwner, USkillNumberBase* SkillComp);
	void TeleportMarkDeactivate();

	bool AttackPushBack(FVector NewLocation);

	void RotateTowardsTarget(AActor *TargetActor, float Speed = -1.0f);

	// HitPoint 관련 함수
	void DistinctHitPoint(FVector ImpactPoint, AActor* HitActor);

	void SetHitEffect(FVector HitLocation);

	// 몬스터 Anim 관련 함수
	UFUNCTION()
	void EnemyMontageEnded(UAnimMontage *Montage, bool bInterrupted);

	// 몬스터 CrowdControl 관련 함수
	UFUNCTION(BlueprintPure)
	bool IsHardCC();

	// 몬스터 어그로
	AActor* IsAggro();

	// 흡혈 영역
	FORCEINLINE bool IsInDrainField() const {return InDrainField;}; // InDrainField 반환;
	FORCEINLINE void AddInDrainField(bool NewBool) { InDrainField = NewBool;};

	// 패트롤 위치
	FVector GetPatrolLocation(FVector Start);

	// Detect Render 스폰
	void SpawnDetectRender();

protected:
	float DamageMath(float Damage);
	bool AttackTrace(FHitResult &HitResult, FVector &HitVector, bool Custom = false, float Radius = 0.0f, FVector CustomStart = FVector(0,0,0), FVector CustomEnd = FVector(0,0,0));
	void ResetHitStiffness();
	void SpawnDamageText(AController* EventInstigator, float Damage, FVector AddLocation);

	//Monster 세팅 초기화
	void InitMonster();
	void StopClear();

	//드롭 관련 함수
	void DropItemActor();

	// 몬스터 Dead 관련 함수
	virtual void EnemyDestroyed();
	void CheckAllEnemyKilled();
	

	// 몬스터 Anim Ended 관련 함수
	void BindMontageEnded();

	void OnNormalAttack_MontageEnded();
	void OnDeath_MontageEnded();
	void OnDetect_MontageEnded();

	// 몬스터 CC기 관련 함수
	void OnPlayerKnockBack(AActor* Actor, float Dist, float Time);
	void OnPlayerStun(AActor *Actor, float Time);
	void CrowdControlCheck();

	// 피격 데미지 배수 관련 함수
	void SubtractDamageMultiple(float Subtract);
	void OnDetachingDamagedMark();

protected:
	UPROPERTY(EditAnywhere, Category = "Status")
	UWidgetComponent* HealthWidgetComp;

	UPROPERTY(EditAnywhere, Category = "Status")
	float Monster_AttackRange = 250.0f;

	UPROPERTY(EditAnywhere, Category = "Status")
	float Monster_AttackRadius = 80.0f;

	UPROPERTY(EditAnywhere, Category = "Status")
	float Distance_Limit = 800.0f;

	UPROPERTY(EditAnywhere, Category = "Status")
	float DetectLimit = 2500.0f;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* TeleportMark;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* DamagedMark;

	bool bKnockBackAttack = false;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> SkinMesh;
	float Apperence = 1.0f;
	UPROPERTY(EditAnywhere)
	float FadeOutSpeed = 1.0f;

	// TimerHandle
	FTimerHandle PresentDamageTimerHandle;
	FTimerHandle TeleportTimerHandle;
	FTimerHandle HitTimerHandle;
	FTimerHandle StopTimerHandle;
	FTimerHandle DamageMultipleTimerHandle;
	FTimerHandle DamagedMarkTimerHandle;
	// 머터리얼

	// 드롭 관련 변수
	// UPROPERTY(EditAnywhere, Category = "Drop")
	// TMap<TSubclassOf<AEnemy_DropBase>, float> DropMap; // Drop Class , Drop 확률 (따로 드랍)

	TWeakObjectPtr<AEnemy_DropPackage> DropPackage;

	UPROPERTY(EditAnywhere)
	bool OnDrop = true;

	TWeakObjectPtr<AActor> MarkActor;

	// 몬스터 Hit 관련 변수
	UPROPERTY(EditAnywhere, Category = "Hit")
	float HitStiffnessTime = 0.75f;

	UPROPERTY(EditAnywhere, Category = "Hit")
	bool IsAttackPush = true;

	float MonsterIsForward = 0.0f;
	float MonsterIsRight = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Hit")
	FVector DamageTextAddLocation = FVector(0,0,0);

	UPROPERTY(EditAnywhere, Category = "Hit")
	UNiagaraSystem* HitEffect_L;

	UPROPERTY(EditAnywhere, Category = "Hit")
	UNiagaraSystem* HitEffect_R;

	UPROPERTY(EditAnywhere, Category = "Hit")
	UNiagaraSystem* HitEffect_B;

	// 몬스터 Anim 관련 변수
	TWeakObjectPtr<UAP_EnemyBaseAnimInstance> EnemyAnim;

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

public:
	FOnHPChanged OnEnemyHPChanged;

};
