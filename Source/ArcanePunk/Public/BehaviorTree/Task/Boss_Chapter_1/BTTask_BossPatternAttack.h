// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Task/BTTask_InCludeWaitBase.h"
#include "BTTask_BossPatternAttack.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API UBTTask_BossPatternAttack : public UBTTask_InCludeWaitBase
{
	GENERATED_BODY()
public:
	UBTTask_BossPatternAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);	

private:
	UPROPERTY(EditAnywhere, Category=Blackboard)
	struct FBlackboardKeySelector CurrentPhase;	
};
