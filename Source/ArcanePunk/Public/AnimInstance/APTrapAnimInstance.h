// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "APTrapAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API UAPTrapAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UAPTrapAnimInstance();
	virtual void NativeBeginPlay() override;

	// Play Anim
	float PlayTrapOperation_Montage();
	float PlayReload_Montage();

	/* Notify */ 
	UFUNCTION()
	void AnimNotify_DamageTrigger();
	UFUNCTION()
	void AnimNotify_SpawnTrapOperationSound();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* TrapOperation_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* TrapReload_Montage;

};
