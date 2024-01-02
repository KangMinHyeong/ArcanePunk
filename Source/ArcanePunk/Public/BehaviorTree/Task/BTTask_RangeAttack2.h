// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RangeAttack2.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API UBTTask_RangeAttack2 : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_RangeAttack2();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);		
};
