// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Common/APCrowdControlComponent.h"
#include "Components/ActorComponent.h"
#include "APAttackComponent.generated.h"

class AAPCharacterBase;
class UArcanePunkCharacterAnimInstance;
class UNiagaraSystem;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSuperiorAttack, float, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANEPUNK_API UAPAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAPAttackComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void InitAttackComp();

	void StartAttack_A(bool & bCanMove);
	void StartAttack_B(bool & bCanMove);

	void SetAttack_A(bool NewBool);
	void SetAttack_B(bool NewBool);

	bool GetAttack_A();
	bool GetAttack_B();

	//Combo
	void ComboAttackStart();
	void ComboAttackEnd();
	void ComboCheck();

	void SpawnSwordTrail(uint8 ComboStack);

	void NormalAttack(FVector Start, bool CloseAttack, float Multiple = 1.0f, bool bStun = false, float StunTime = 0.0f, bool Custom = false, float CustomRadius = 0.0f); 	//Attack 트리거 발동
	
	void MultiAttack();
	void MultiAttack(FVector Start, bool CloseAttack, float Multiple = 1.0f, bool bStun = false, float StunTime = 0.0f, bool Custom = false, float CustomRadius = 0.0f);
	void MultiAttack(FVector Start, FVector End, float Radius, float Multiple = 1.0f, uint8 HitNumbers = 1, bool bStun = false, float StunTime = 0.0f);
	void MultiAttack(FVector Start, FVector End, float Radius, float Multiple = 1.0f, uint8 HitNumbers = 1, float InstantDeathPercent = 0.0f);
	TArray<AActor*> MultiAttack_Return(FVector Start, FVector End, float Radius, float Multiple = 1.0f, uint8 HitNumbers = 1, float InstantDeathPercent = 0.0f,bool bStun = false, float StunTime = 0.0f);
	
	void MultiAttack_KnockBack(FVector Start, FVector End, float Radius, float KnockBackDist, float Multiple = 1.0f, uint8 HitNumbers = 1, float InstantDeathPercent = 0.0f, float KnockBackTime = 0.0f,  bool PlayerKnockBack = false);
	TArray<AActor*> MultiAttack_KnockBack_Return(FVector Start, FVector End, float Radius, float KnockBackDist, float Multiple = 1.0f, uint8 HitNumbers = 1, float InstantDeathPercent = 0.0f, float KnockBackTime = 0.0f,  bool PlayerKnockBack = false);
	
	void MultiAttack_Burn(FVector Start, FVector End, float Radius, float DOT, float TotalTime, float InRate);
	void MultiAttack_Slow(FVector Start, FVector End, float Radius, int32 SlowPercent,float TotalTime);
	void MultiAttack_OnlyCC(FVector Start, FVector End, float Radius, ECharacterState UpdateState, float TotalTime);
	
	FORCEINLINE float GetMaxDistance() const {return MaxDistance;}; // 공격 사거리 반환

	FORCEINLINE float GetBaseInstantDeathPercent() const {return BaseInstantDeathPercent;};
	FORCEINLINE void SetBaseInstantDeathPercent(float NewValue) {BaseInstantDeathPercent = NewValue;}; //

	FORCEINLINE float GetDrainCoefficient() const {return DrainCoefficient;};
	FORCEINLINE void SetDrainCoefficient(float NewValue) {DrainCoefficient = NewValue;}; //
	FORCEINLINE void AddDrainCoefficient(float NewValue) {DrainCoefficient+=NewValue;}; //

	FORCEINLINE float GetFieldDrainCoefficient() const {return FieldDrainCoefficient;};
	FORCEINLINE void SetFieldDrainCoefficient(float NewValue) {FieldDrainCoefficient = NewValue;}; //
	FORCEINLINE float GetSkillDrainCoefficient() const {return SkillDrainCoefficient;};
	FORCEINLINE void SetSkillDrainCoefficient(float NewValue) {SkillDrainCoefficient = NewValue;}; //

	FORCEINLINE float GetBaseAttackDist() const {return BaseAttackDist;};
	FORCEINLINE float GetBaseAttackRadius() const {return BaseAttackRadius;};

	FORCEINLINE void SetSuperiorMode(bool NewBool) {bSuperiorMode = NewBool;};

	float ApplyDamageToActor(AActor* DamagedActor, float Damage, FHitResult HitResult, bool bCriticalApply);
	void DrainCheck(AActor* DamagedActor, float DamageApplied, float Coeff);

private:
	bool AttackTrace(FHitResult &HitResult, FVector &HitVector, FVector Start, bool CloseAttack, bool Custom = false, float CustomRadius = 0.0f); // Attack Trace
	bool MultiAttackTrace(TArray<FHitResult> &HitResult, FVector &HitVector, FVector Start, bool CloseAttack, bool Custom = false, float CustomRadius = 0.0f); // Attack Trace
	bool MultiAttackTrace(TArray<FHitResult> &HitResult, FVector &HitVector, FVector Start, FVector End, float Radius, bool ExceptPlayer = true); // Attack Trace
	
	void ApplyDamageToActor(AActor* DamagedActor, float Damage, FPointDamageEvent myDamageEvent, AController* MyController, uint8 HitNumbers);

private:
	TWeakObjectPtr<AAPCharacterBase> OwnerCharacter;
	TWeakObjectPtr<UArcanePunkCharacterAnimInstance> OwnerAnim;
	
	bool bAttack_A = false;
	bool bAttack_B = false;

	//Combo
	bool CanCombo = true;
	bool IsComboInputOn = false;

	int32 CurrentCombo = 0;
	int32 MaxCombo = 3;

	int32 HitNum = 1;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float MaxDistance = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackRadius = 40.0f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* ComboHitEffect;

	UPROPERTY(EditAnywhere, Category = "Attack")
	FVector HitEffectScale = FVector(1,1,1);

	UPROPERTY(EditAnywhere, Category = "Attack")
	float BaseAttackRadius = 60.0f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float BaseAttackDist = 150.0f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float BaseInstantDeathPercent = 0.0f;

	float DrainCoefficient = 0.0f;
	float FieldDrainCoefficient = 0.0f;
	float SkillDrainCoefficient = 0.0f;

	bool bSuperiorMode = false;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UNiagaraSystem* SwordTrailEffect;

	UPROPERTY(EditAnywhere, Category = "Attack")
	FRotator Combo_1_Rot = FRotator::ZeroRotator;
	UPROPERTY(EditAnywhere, Category = "Attack")
	FRotator Combo_2_Rot = FRotator::ZeroRotator;
	UPROPERTY(EditAnywhere, Category = "Attack")
	FRotator Combo_3_Rot = FRotator::ZeroRotator;
	UPROPERTY(EditAnywhere, Category = "Attack")
	FVector SwordTrailHeight = FVector::ZeroVector;

public:
	FOnSuperiorAttack OnSuperiorAttack;
};
