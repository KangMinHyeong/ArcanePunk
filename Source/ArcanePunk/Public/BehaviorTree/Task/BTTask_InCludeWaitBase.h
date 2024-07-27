// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"
#include "BTTask_InCludeWaitBase.generated.h"

class UAP_EnemyBaseAnimInstance;
class AEnemy_CharacterBase;

UCLASS()
class ARCANEPUNK_API UBTTask_InCludeWaitBase : public UBTTask_Wait
{
	GENERATED_BODY()
public:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	void SetAIOwner(UBehaviorTreeComponent &OwnerComp);
	void CalculateWaitTime_Montage(UAnimSequenceBase* AnimSequence);

protected:
	float DelayTime = 0.0f;

	TWeakObjectPtr<AEnemy_CharacterBase> Enemy;
	TWeakObjectPtr<UAP_EnemyBaseAnimInstance> EnemyAnim;

	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category=Blackboard)
	struct FBlackboardKeySelector BlackboardKey;
};
