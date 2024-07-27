// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/BTTask_SpawnAttackRange.h"

#include "AIController.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "AnimInstance/AP_EnemyBaseAnimInstance.h"

UBTTask_SpawnAttackRange::UBTTask_SpawnAttackRange()
{
    NodeName = TEXT("Spawn Attack Range");
}

EBTNodeResult::Type UBTTask_SpawnAttackRange::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{    
    SetAIOwner(OwnerComp);
    if(!Enemy.IsValid()) return EBTNodeResult::Failed;
	if(!EnemyAnim.IsValid()) return EBTNodeResult::Failed;

    WaitTime = DelayTime + Enemy->GetAttackRangeTime() + FMath::RandRange(-RandomDeviation, RandomDeviation);
        
    Enemy->SpawnAttackRange();
    Super::ExecuteTask(OwnerComp, NodeMemory);

    return EBTNodeResult::InProgress;
}