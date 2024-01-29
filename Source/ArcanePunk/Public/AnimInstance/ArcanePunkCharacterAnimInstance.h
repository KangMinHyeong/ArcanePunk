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
	void PlaySkill_1_Montage();
	void PlaySkill_2_Montage();
	void PlaySkill_3_Montage();
	void PlaySkill_4_Montage();
	void PlaySkill_5_Montage();
	void PlaySkill_6_Montage();
	void PlaySkill_7_Montage();
	void PlayUltSkill_1_Montage();

	UFUNCTION()
	void AnimNotify_AttackTrigger();
	UFUNCTION()
	void AnimNotify_Active_Q(); // Skill_1
	UFUNCTION()
	void AnimNotify_Skill_E_Trigger(); // Skill_2
	UFUNCTION()
	void AnimNotify_Skill_4_Trigger();
	UFUNCTION()
	void AnimNotify_Skill_5_Trigger();
	UFUNCTION()
	void AnimNotify_Skill_5_FireStart();
	UFUNCTION()
	void AnimNotify_Skill_5_Fire();
	UFUNCTION()
	void AnimNotify_Skill_5_FireEnd();
	UFUNCTION()
	void AnimNotify_Skill_6_Trigger();
	UFUNCTION()
	void AnimNotify_Skill_7_Trigger();

	UFUNCTION()
	void AnimNotify_UltSkill_1_Trigger();

	UFUNCTION()
	void AnimNotify_NextCombo();
	UFUNCTION()
	void AnimNotify_AnimMove();
	UFUNCTION()
	void AnimNotify_AnimStop();
	UFUNCTION()
	void AnimNotify_FootRight();
	UFUNCTION()
	void AnimNotify_FootLeft();

	void JumpToComboSection(int32 NewSection);

	FName GetAttackMontageSectionName(int32 Section);

	int32 GetAttackSection();

	void FireCheck();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	bool IsDead;

	FVector Before = FVector(0,0,0);
    FVector NowLocation = FVector(0,0,0);

	int32 AttackSection = 0;

public:
	FOnComboCheckDelegate OnComboCheck;
	FOnMontageEnded OnAttack_A_Montage_End;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Attack_A_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Attack_B_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Skill_1_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Skill_2_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Skill_3_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Skill_4_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Skill_5_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Skill_6_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Skill_7_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* UltSkill_1_Montage;

	FTimerHandle Skill5_FireTimerHandle;
};
