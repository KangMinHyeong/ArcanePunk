// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RushAttackEnd.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API UBTTask_RushAttackEnd : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_RushAttackEnd();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);	
};
