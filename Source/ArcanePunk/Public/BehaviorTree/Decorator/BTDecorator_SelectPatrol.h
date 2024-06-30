// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_SelectPatrol.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API UBTDecorator_SelectPatrol : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTDecorator_SelectPatrol();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere)
	float PatrolSpeed = 200.0f;
};
