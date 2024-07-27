// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"
#include "BTTask_AttackRotate.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API UBTTask_AttackRotate : public UBTTask_Wait
{
	GENERATED_BODY()
public:
	UBTTask_AttackRotate();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);	

private:
	UPROPERTY(EditAnywhere)
	float RotateSpeed = 450.0f;

	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category=Blackboard)
	struct FBlackboardKeySelector BlackboardKey;
};
