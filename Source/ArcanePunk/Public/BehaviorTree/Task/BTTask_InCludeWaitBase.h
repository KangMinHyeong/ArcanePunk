// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"
#include "BTTask_InCludeWaitBase.generated.h"

class UAP_EnemyBaseAnimInstance;
class AEnemy_CharacterBase;
class AEnemy_BossBase;

UCLASS()
class ARCANEPUNK_API UBTTask_InCludeWaitBase : public UBTTask_Wait
{
	GENERATED_BODY()
public:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	void SetAIOwner(UBehaviorTreeComponent &OwnerComp);
	void SetAIOwner_Boss(UBehaviorTreeComponent &OwnerComp);
	void CalculateWaitTime_Montage(float AnimSequenceTime);

protected:
	float DelayTime = 0.0f;

	TWeakObjectPtr<AEnemy_CharacterBase> Enemy;
	TWeakObjectPtr<AEnemy_BossBase> Boss;
	TWeakObjectPtr<UAP_EnemyBaseAnimInstance> EnemyAnim;

	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category=Blackboard)
	struct FBlackboardKeySelector BlackboardKey;


};
