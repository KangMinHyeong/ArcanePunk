// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Task/BTTask_InCludeWaitBase.h"
#include "BTTaskNode_NormalAttack.generated.h"

UCLASS()
class ARCANEPUNK_API UBTTaskNode_NormalAttack : public UBTTask_InCludeWaitBase
{
	GENERATED_BODY()
public:
	UBTTaskNode_NormalAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
