// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_MoveToCheckDist.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API UBTTask_MoveToCheckDist : public UBTTask_MoveTo
{
	GENERATED_BODY()
public:
	UBTTask_MoveToCheckDist();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);	
		
};
