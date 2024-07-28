// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AP_EnemyBaseAnimInstance.generated.h"

class AEnemy_CharacterBase;

UCLASS()
class ARCANEPUNK_API UAP_EnemyBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UAP_EnemyBaseAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	FORCEINLINE bool IsBattleMode() {return bBattleMode;};
	FORCEINLINE void SetbBattleMode(bool NewBool) {bBattleMode = NewBool;};

	UFUNCTION(BlueprintPure)
	bool IsRun();

	// Play Anim
	void PlayNormalAttack_Montage();
	void PlayDeath_Montage();
	void PlayHit_Montage();
	void PlayDetect_Montage();
	UAnimMontage* PlayRandomIdle_Montage();

	/* Notify */ 
	UFUNCTION()
	void AnimNotify_NormalAttack();

	// ScoutDog
	UFUNCTION()
	void AnimNotify_JumpEffect();
	UFUNCTION()
	void AnimNotify_LeapFoward();
	UFUNCTION()
	void AnimNotify_LeapEnd();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	bool bBattleMode = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	bool IsDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	class UAnimSequence* RunPose;

	TWeakObjectPtr<AEnemy_CharacterBase> Enemy;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* NormalAttack_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Death_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Hit_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Detect_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Idle", Meta = (AllowPrivateAccess = true))
	TArray<UAnimMontage*> Idle_Montages;
};
