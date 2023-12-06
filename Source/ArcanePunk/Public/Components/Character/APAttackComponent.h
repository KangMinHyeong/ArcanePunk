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

	void NormalAttack(FVector Start, bool CloseAttack, float Multiple = 1.0f); 	//Attack 트리거 발동

private:
	bool AttackTrace(FHitResult &HitResult, FVector &HitVector, FVector Start, bool CloseAttack); // Attack Trace

private:
	bool bAttack_A = false;
	bool bAttack_B = false;

	//Combo
	bool CanCombo = true;
	bool IsComboInputOn = false;

	int32 CurrentCombo = 0;
	int32 MaxCombo = 3;

	//Attack Variable
	UPROPERTY(EditAnywhere)
	float MaxDistance = 200.0f;

	UPROPERTY(EditAnywhere)
	float AttackRadius = 40.0f;

};
