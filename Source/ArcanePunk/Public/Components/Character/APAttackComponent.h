// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "APAttackComponent.generated.h"


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

	void NormalAttack(FVector Start, bool CloseAttack, float Multiple = 1.0f, bool bStun = false, float StunTime = 0.0f, bool Custom = false, float CustomRadius = 0.0f); 	//Attack 트리거 발동
	void MultiAttack(FVector Start, bool CloseAttack, float Multiple = 1.0f, bool bStun = false, float StunTime = 0.0f, bool Custom = false, float CustomRadius = 0.0f);
	void MultiAttack(FVector Start, FVector End, float Radius, float Multiple = 1.0f, uint8 HitNumbers = 1, bool bStun = false, float StunTime = 0.0f);
	void MultiAttack_KnockBack(FVector Start, FVector End, float Radius, float KnockBackDist, float Multiple = 1.0f, uint8 HitNumbers = 1, float KnockBackTime = 0.0f,  bool PlayerKnockBack = false);
	
	FORCEINLINE float GetMaxDistance() const {return MaxDistance;}; // 공격 사거리 반환
private:
	bool AttackTrace(FHitResult &HitResult, FVector &HitVector, FVector Start, bool CloseAttack, bool Custom = false, float CustomRadius = 0.0f); // Attack Trace
	bool MultiAttackTrace(TArray<FHitResult> &HitResult, FVector &HitVector, FVector Start, bool CloseAttack, bool Custom = false, float CustomRadius = 0.0f); // Attack Trace
	bool MultiAttackTrace(TArray<FHitResult> &HitResult, FVector &HitVector, FVector Start, FVector End, float Radius); // Attack Trace
	
	void ApplyDamageToActor(AActor* DamagedActor, float Damage, FPointDamageEvent myDamageEvent, AController* MyController, uint8 HitNumbers);

private:
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

};
