// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_KnockBackAttack.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API UBTTask_KnockBackAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_KnockBackAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);	
};
