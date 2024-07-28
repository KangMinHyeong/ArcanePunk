// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RangeAttack1.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API UBTTask_RangeAttack1 : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_RangeAttack1();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);	
};
