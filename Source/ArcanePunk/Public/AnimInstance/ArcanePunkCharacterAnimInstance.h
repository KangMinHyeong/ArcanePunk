// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ArcanePunkCharacterAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnComboCheckDelegate);

UCLASS()
class ARCANEPUNK_API UArcanePunkCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UArcanePunkCharacterAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void SetDeadAnim() { IsDead = true; }

	void PlayAttack_A_Montage();
	void PlayAttack_B_Montage();
	void PlaySkill_Q_Montage();
	void PlaySkill_E_Montage();
	void PlaySkill_R_Montage();

	UFUNCTION()
	void AnimNotify_AttackTrigger();
	UFUNCTION()
	void AnimNotify_Active_Q();
	UFUNCTION()
	void AnimNotify_Skill_E_Trigger();
	UFUNCTION()
	void AnimNotify_NextCombo();

	void JumpToComboSection(int32 NewSection);

	FName GetAttackMontageSectionName(int32 Section);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	bool IsDead;

	FVector Before = FVector(0,0,0);
    FVector NowLocation = FVector(0,0,0);

public:
	FOnComboCheckDelegate OnComboCheck;
	FOnMontageEnded OnAttack_A_Montage_End;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Attack_A_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Attack_B_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Skill_Q_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Skill_E_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Skill_R_Montage;
};
