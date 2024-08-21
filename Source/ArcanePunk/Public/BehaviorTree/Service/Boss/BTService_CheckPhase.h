// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckPhase.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API UBTService_CheckPhase : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_CheckPhase();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;		
	
};
