// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_PillarThrowCheck.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API UBTDecorator_PillarThrowCheck : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTDecorator_PillarThrowCheck();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere)
	float ThrowDist = 1000.0f;
};
