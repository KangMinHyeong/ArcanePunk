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
	
private:
	bool AttackTrace(FHitResult &HitResult, FVector &HitVector, FVector Start, bool CloseAttack, bool Custom = false, float CustomRadius = 0.0f); // Attack Trace
	bool MultiAttackTrace(TArray<FHitResult> &HitResult, FVector &HitVector, FVector Start, bool CloseAttack, bool Custom = false, float CustomRadius = 0.0f); // Attack Trace
	bool MultiAttackTrace(TArray<FHitResult> &HitResult, FVector &HitVector, FVector Start, FVector End, float Radius); // Attack Trace
	
	void ApplyDamageToActor(AActor* DamagedActor, float Damage, FPointDamageEvent myDamageEvent, AController* MyController, uint8 HitNumbers);

private:
	UPROPERTY()
	bool bAttack_A = false;
	UPROPERTY()
	bool bAttack_B = false;

	//Combo
	UPROPERTY()
	bool CanCombo = true;
	UPROPERTY()
	bool IsComboInputOn = false;

	UPROPERTY()
	int32 CurrentCombo = 0;
	UPROPERTY()
	int32 MaxCombo = 3;

	UPROPERTY()
	int32 HitNum = 1;

	UPROPERTY()
	class UAPHitPointComponent* HitPointComp;

};
