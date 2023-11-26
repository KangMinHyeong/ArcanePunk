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

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	bool IsDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	class UAnimSequence* IdlePose;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	class UAnimSequence* RunPose;
};
