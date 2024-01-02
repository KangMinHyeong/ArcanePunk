// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstance/AP_EnemyBaseAnimInstance.h"
#include "AP_EnemyBossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API UAP_EnemyBossAnimInstance : public UAP_EnemyBaseAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// Play Skill Anim
	void PlayMismatchedAttack_Montage();
	void PlayKnockBackAttack_Montage();
	void PlayStrongAttack_Montage();
	void PlaySpawnMonster_Montage();
	void PlayDrainMonster_Montage();
	void PlayRangeAttack1_Montage();
	void PlayRangeAttack2_Montage();

	UFUNCTION()
	void AnimNotify_MismatchedAttack();
	UFUNCTION()
	void AnimNotify_AttackMove();
	UFUNCTION()
	void AnimNotify_AttackMoveEnd();
	UFUNCTION()
	void AnimNotify_KnockBackAttack();
	UFUNCTION()
	void AnimNotify_SpawnFastSlash();
	UFUNCTION()
	void AnimNotify_SpawnMonster();
	UFUNCTION()
	void AnimNotify_RangeAttack1();
	UFUNCTION()
	void AnimNotify_RangeAttack2();

private:
	FName GetAttackMontageSectionName(uint8 Section);

private:
	uint8 MismatchSection = 1;;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* RushAttack_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* MismatchedAttack_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* KnockBackAttack_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Strong_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* SpawnMonster_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* DrainMonster_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* RangeAttack1_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* RangeAttack2_Montage;
};
