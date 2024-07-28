// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Task/BTTask_InCludeWaitBase.h"
#include "BTTask_Detect.generated.h"

UCLASS()
class ARCANEPUNK_API UBTTask_Detect : public UBTTask_InCludeWaitBase
{
	GENERATED_BODY()
public:
	UBTTask_Detect();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);	
	
};
