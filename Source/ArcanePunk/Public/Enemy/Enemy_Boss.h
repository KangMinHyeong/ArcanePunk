#pragma once

#include "CoreMinimal.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Enemy_Boss.generated.h"

class UParticleSystemComponent;
class UAP_EnemyBossAnimInstance;
class UCapsuleComponent;
class UNiagaraSystem;
class AAPSpawnPointBase;
class UParticleSystem;
class ASwordImpact;
class AAPHUD;
class UAPSpawnMonsterComponent;

UCLASS()
class ARCANEPUNK_API AEnemy_Boss : public AEnemy_CharacterBase
{
	GENERATED_BODY()
public:
	AEnemy_Boss();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	
	// Possess
	UFUNCTION(BlueprintPure)
	bool OnPossessing();

	// RushAttack 관련 함수
	void RushAttackStart(); // BTTask에서 사용 , 돌진 공격
	void RushAttackEnd();
	FORCEINLINE bool GetRushAttack() {return bRushAttack;}; // bRushAttack 반환

	//KnockBackAttack 관련 함수
	void KnockBackAttackReady();
	void KnockBackAttack();
	void KnockBackActivate();

	//StrongAttack 관련 함수
	void SpawnFastSlash();

	//SpawnMonster 관련 함수
	void SpawnLocationEffect();
	
	//Range Attack 1 관련 함수
	void RangeAttack_1_Ready();
	void SpawnRangeAttack_1();

	//Range Attakc 2 관련 함수
	void RangeAttack_2_Ready();
	void SpawnRangeAttack_2();

	// Attack Move 관련 함수
	FORCEINLINE void SetAttackMove(bool NewBool) { bAttackMove = NewBool;}; //bAttackMove 설정

	// Pattern Num 관련함수
	int32 GetPatternNum();

	UFUNCTION(BlueprintPure)
	bool SetHPUI();

protected:
	// 몽타주 엔드 콜백 함수
	void BindMontageEnd();
	UFUNCTION()
	void BossMontageEnded(UAnimMontage *Montage, bool bInterrupted);
	void OnRushAttack_MontageEnd();
	void OnMismatchedAttack_MontageEnd();
	void OnKnockBackAttack_MontageEnd();
	void OnStrongAttack_MontageEnd();
	void OnSpawnMonster_MontageEnd();
	void OnRangeAttack1_MontageEnd();
	void OnRangeAttack2_MontageEnd();
	void OnDrainMonster_MontageEnd();

	// RushAttack 관련 함수
	void BindRushAttack();
	void ActiveRushEffect();
	UFUNCTION()
	virtual void ActiveRushTrigger(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	// SpawnMonster 관련 함수
	void SpawnMonster();
	void DrainMonster();

	//Range Attack 1 관련 함수
	void RangeAttack_1();

	//Range Attack 2 관련 함수
	void RangeAttack_2();
	void OnSwordImpactSpawn(float AddAngle = 0.0f);

	// Attack Move 관련 함수
	void OnAttackMove(float DeltaTime);
	void OnHitPlayerMove(FVector MoveLocation);

	// Stop 관련 함수
	void StopEnd();

	// HPUI Set
	virtual void EnemyDestroyed() override; // Dead + ClearHPUI

	// Pattern Num 관련함수
	void InitPatternNums();

private:
	// 부착 Component
	UPROPERTY(EditAnywhere, Category = "Component")
	UAPSpawnMonsterComponent* SpawnMonsterComp;

	//Possess
	UPROPERTY(EditAnywhere, Category = "Possess")
	AEnemyBaseAIController* MonsterAIController;
	
	// Boss Status 관련 변수
	UPROPERTY(EditAnywhere, Category = "Status")
	float DefaultSpeed = 0.0f;

	// Boss Effect 관련 변수
	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystemComponent* RushEffect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystem* TestRushEffect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	UNiagaraSystem* KnockBackSlash;

	UPROPERTY(EditAnywhere, Category = "Effect")
	UNiagaraSystem* FastSlash;
	
	UPROPERTY(EditAnywhere, Category = "Effect")
	UNiagaraSystem* DrainEffect;

	// Boss Material 관련 변수
	UPROPERTY(EditAnywhere, Category = "Material")
	TArray<UMaterial*> PatternMaterial;

	// Boss Animation 관련 변수
	UPROPERTY()
	UAP_EnemyBossAnimInstance* OwnerAnim;

	// Rush Attack 관련 변수
	UPROPERTY(EditAnywhere, Category = "RushAttack")
	UCapsuleComponent* RushTrigger;

	UPROPERTY(EditAnywhere, Category = "RushAttack")
	float RushAttackTime = 10.0f;
	
	UPROPERTY()
	bool bRushAttack = false;

	UPROPERTY(EditAnywhere, Category = "RushAttack")
	float RushCoefficient = 1.0f;

	UPROPERTY(EditAnywhere, Category = "RushAttack")
	float RushKnockBackTime = 2.5f;

	// KnockBack Attack 관련 변수
	UPROPERTY(EditAnywhere, Category = "KnockBackAttack")
	float KnockBackRangeCoefficient = 1.4f;

	// SpawnMonster 관련 변수
	UPROPERTY(EditAnywhere, Category = "SpawnMonster")
	TSubclassOf<AAPSpawnPointBase> SpawnPointClass;

	UPROPERTY(EditAnywhere, Category = "SpawnMonster")
	TSubclassOf<AEnemy_CharacterBase> SpawnMonsterClass;

	UPROPERTY(EditAnywhere, Category = "SpawnMonster")
	float SpawnMonsterNum = 3;

	UPROPERTY(EditAnywhere, Category = "SpawnMonster")
	float SpawnMonsterTime = 3.0f;

	UPROPERTY(EditAnywhere, Category = "SpawnMonster")
	float DrainMonsterTime = 10.0f;
	
	UPROPERTY()
	TArray<AActor*> SpawnLocation;

	UPROPERTY()
	TArray<AEnemy_CharacterBase*> MonsterArr;

	// Range Attack 1 관련 변수
	UPROPERTY(EditAnywhere, Category = "RangeAttack_1")
	TSubclassOf<AAPSpawnPointBase> SpawnRangeAttackPointClass;

	UPROPERTY(EditAnywhere, Category = "RangeAttack_1")
	float RangeAttack_1_ReadyTime = 1.5f;

	UPROPERTY()
	TArray<AAPSpawnPointBase*> SpawnRangeAttackLocation;

	UPROPERTY(EditAnywhere, Category = "RangeAttack_1")
	float RangeAttack_1_Coefficient = 2.0f;

	UPROPERTY(EditAnywhere, Category = "RangeAttack_1")
	UParticleSystem* RangeAttack_1_Impact;

	UPROPERTY(EditAnywhere, Category = "RangeAttack_1")
	bool RandomRangeAttack = false;

	UPROPERTY(EditAnywhere, Category = "RangeAttack_1")
	int32 RangeAttackNum = 3;

	UPROPERTY(EditAnywhere, Category = "RangeAttack_1")
	float RangeAttackDist = 1000.0f;

	// Range Attack 2 관련 변수
	UPROPERTY(EditAnywhere, Category = "RangeAttack_2")
	TSubclassOf<ASwordImpact> RangeAttack2Class;

	UPROPERTY(EditAnywhere, Category = "RangeAttack_2")
	float RangeAttack_2_ReadyTime = 2.0f;

	UPROPERTY(EditAnywhere, Category = "RangeAttack_2")
	bool ForwardDirection = true;

	UPROPERTY(EditAnywhere, Category = "RangeAttack_2")
	bool DiagonalDirection = false;

	UPROPERTY()
	int32 RA_Num = 0;
	
	//Attack Move 관련 변수
	UPROPERTY()
	bool bAttackMove = false;

	UPROPERTY(EditAnywhere, Category = "AttackMove")
	float AttackMoveSpeed = 250.0f;

	// TimerHandle
	FTimerHandle RushAttackTimerHandle;
	FTimerHandle StopTimerHandle;
	FTimerHandle SpawnMonsterTimerHandle;
	FTimerHandle DrainMonsterTimerHandle;
	FTimerHandle RangeAttack_1_ReadyTimerHandle;
	FTimerHandle RangeAttack_2_ReadyTimerHandle;

	// Stop 관련 변수
	UPROPERTY(EditAnywhere, Category = "Stop")
	float StopTime = 1.1f;

	// UPROPERTY()
	// bool bDoing = false;

	// Test Rotator
	UPROPERTY(EditAnywhere, Category = "Test Rotator")
	FRotator KnockBackSlashRotator = FRotator(0,0,0);

	UPROPERTY(EditAnywhere, Category = "Test Rotator")
	FRotator FastSlashRotator = FRotator(0,0,0);

	//AAPHUD
	UPROPERTY()
	AAPHUD* PlayerHUD;

	// Pattern Num
	UPROPERTY()
	TArray<int32> PatternNums;

	UPROPERTY(EditAnywhere)
	int32 BossSkillNum = 7;
};
