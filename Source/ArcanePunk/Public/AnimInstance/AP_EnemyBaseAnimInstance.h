// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AP_EnemyBaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API UAP_EnemyBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UAP_EnemyBaseAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintPure)
	bool IsRun();

	// Play Skill Anim
	void PlayNormalAttack_Montage();

	UFUNCTION()
	void AnimNotify_NormalAttack();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	bool IsDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	class UAnimSequence* RunPose;

private:
	UPROPERTY()
	class AEnemy_CharacterBase* Enemy;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* NormalAttack_Montage;
};
