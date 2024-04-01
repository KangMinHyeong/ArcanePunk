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

	FORCEINLINE void SetbBattleMode(float NewValue) {bBattleMode = NewValue;};

	UFUNCTION(BlueprintPure)
	bool IsRun();

	// Play Skill Anim
	void PlayNormalAttack_Montage();

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
	float bBattleMode = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	bool IsDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	class UAnimSequence* RunPose;

	TWeakObjectPtr<AEnemy_CharacterBase> Enemy;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* NormalAttack_Montage;
};
