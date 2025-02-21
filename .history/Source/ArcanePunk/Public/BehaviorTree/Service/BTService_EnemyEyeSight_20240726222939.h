// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_EnemyEyeSight.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEPUNK_API UBTService_EnemyEyeSight : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_EnemyEyeSight();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;	

	AActor* GetPlayerActor(AActor* AIOwner);

private:
	UPROPERTY(EditAnywhere)
	bool BattleLoopCondition = false;

	bool IsLoop = false;
};
