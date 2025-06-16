// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ArcanePunkCharacterAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnComboCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnUltChargeEnd);

class AArcanePunkCharacter;
class AAPCharacterBase;

UCLASS()
class ARCANEPUNK_API UArcanePunkCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UArcanePunkCharacterAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeBeginPlay() override;

	void SetDeadAnim() { IsDead = true; }

	void PlayCombo_Montage(float AttackCancelTime = 0.0f);
	void PlayParrying_Montage();
	void PlayParryingSuccess_Montage();
	void StopComboAttack();

	void PlayDash_Montage();
	void StopDash_Montage();

	void PlaySwapSkill_Exit();
	void PlaySwapSkill_Entry();

	void PlaySkill_1_Montage();
	void PlaySkill_2_Montage();
	void PlaySkill_3_Montage();
	void PlaySkill_4_Montage();
	void PlaySkill_5_Montage();
	void PlaySkill_6_Montage();
	void PlaySkill_7_Montage();
	void PlaySkill_8_Montage();
	void PlaySkill_9_Montage();
	void PlaySkill_10_Montage();
	void PlaySkill_11_Montage();
	void PlaySkill_12_Montage();
	void PlaySkill_13_Montage();
	void PlaySkill_14_Montage();
	void PlaySkill_15_Montage();
	void PlaySkill_18_Montage();
	void PlaySkill_19_Montage();
	void PlaySkill_20_Montage();

	void StopSkill_5_Montage();
	void StopSkill_5_Fire_Montage();
	void StopSkill_18_Montage();
	
	void PlaySkill_5_Fire_Montage();

	void PlayUltSkill_1_Montage();
	void PlayUltSkill_2_Montage();
	void PlayUltSkill_3_Montage();
	void PlayUltSkill_4_Montage(); 
	void PlayUltSkill_5_Montage();
	void PlayUltSkill_6_Montage();
	void PlayUltSkill_8_Montage();
	void PlayUltSkill_9_Montage();
	void PlayUltSkill_10_Montage();
	void PlayUltSkill_11_Montage();
	void PlayUltSkill_12_Montage();
	void PlayUltSkill_13_Montage();
	void PlayUltSkill_14_Montage();
	void PlayUltSkill_15_Montage();
	void PlayUltSkill_17_Montage();

	void StopUltSkill_3_Montage();
	void StopUltSkill_6_Montage();
	void StopUltSkill_11_Montage();
	void StopUltSkill_12_Montage();

	void JumpToFireMontage_Ult();

	UFUNCTION()
	void AnimNotify_AttackTrigger();
	UFUNCTION()
	void AnimNotify_MultiAttackTrigger();
	UFUNCTION()
	void AnimNotify_SwordTrail_1();
	UFUNCTION()
	void AnimNotify_SwordTrail_2();
	UFUNCTION()
	void AnimNotify_SwordTrail_3();

	UFUNCTION()
	void AnimNotify_SpawnSwordSound_1();
	UFUNCTION()
	void AnimNotify_SpawnSwordSound_2();
	UFUNCTION()
	void AnimNotify_SpawnSwordSound_3();
	
	UFUNCTION()
	void AnimNotify_AttackVoiceSound();

	UFUNCTION()
	void AnimNotify_Skill_Trigger();
	UFUNCTION()
	void AnimNotify_Skill_Charging();
	UFUNCTION()
	void AnimNotify_Skill_ChargingEnd();
	UFUNCTION()
	void AnimNotify_Skill_End();
	UFUNCTION()
	void AnimNotify_Skill_Enhance();

	UFUNCTION()
	void AnimNotify_UltSkill_Trigger();
	UFUNCTION()
	void AnimNotify_UltSkill_Charge_Trigger();

	UFUNCTION()
	void AnimNotify_NextCombo();
	UFUNCTION()
	void AnimNotify_AnimMove();
	UFUNCTION()
	void AnimNotify_AnimStop();
	UFUNCTION()
	void AnimNotify_FootRight();
	UFUNCTION()
	void AnimNotify_FootRightOnlySound();
	UFUNCTION()
	void AnimNotify_FootLeft();
	UFUNCTION()
	void AnimNotify_FootLeftOnlySound();

	UFUNCTION()
	void AnimNotify_HideClear();

	void JumpToComboSection(int32 NewSection);

	FName GetAttackMontageSectionName(int32 Section);

	int32 GetAttackSection();

	void FireCheck();
	

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float bBattleMode = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	bool IsDead;

	FVector Before = FVector(0,0,0);
    FVector NowLocation = FVector(0,0,0);

	int32 AttackSection = 0;

	TWeakObjectPtr<AAPCharacterBase> OwnerCharacter;
	TWeakObjectPtr<AArcanePunkCharacter> OwnerPlayer;

public:
	FOnComboCheckDelegate OnComboCheck;
	FOnMontageEnded OnAttack_A_Montage_End;
	FOnUltChargeEnd OnUltChargeEnd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Combo_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Parrying_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* ParryingSuccess_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Dash_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* SwapSkill_Retreat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* SwapSkill_Sally;

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
	UAnimMontage* Skill_8_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Skill_9_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Skill_10_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Skill_11_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Skill_12_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Skill_13_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Skill_14_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Skill_15_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Skill_18_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Skill_19_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Skill_20_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Skill_5_Fire_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* UltSkill_1_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* UltSkill_2_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* UltSkill_3_Montage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* UltSkill_4_Montage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* UltSkill_5_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* UltSkill_6_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* UltSkill_8_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* UltSkill_9_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* UltSkill_10_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* UltSkill_11_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* UltSkill_12_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* UltSkill_13_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* UltSkill_14_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* UltSkill_15_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* UltSkill_17_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* UltSkill_Fire_Montage;

	FTimerHandle Skill5_FireTimerHandle;
};
