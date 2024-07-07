// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RandomPlayNumber.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API UBTTask_RandomPlayNumber : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_RandomPlayNumber();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);	

private:
	UPROPERTY(EditAnywhere)
	uint8 MaxNumber = 2;	
};
