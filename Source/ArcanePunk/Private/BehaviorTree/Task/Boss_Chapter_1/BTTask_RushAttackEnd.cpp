// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/Boss_Chapter_1/BTTask_RushAttackEnd.h"

#include "AIController.h"
#include "Enemy/Enemy_Boss.h"

UBTTask_RushAttackEnd::UBTTask_RushAttackEnd()
{
    NodeName = TEXT("Rush Attack End");
}

EBTNodeResult::Type UBTTask_RushAttackEnd::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    if(!OwnerComp.GetAIOwner()) return EBTNodeResult::Failed;


    AEnemy_Boss* Boss = Cast<AEnemy_Boss>(OwnerComp.GetAIOwner()->GetPawn());
    if(!Boss) return EBTNodeResult::Failed;
    
    Boss->RushAttackStart();

    return EBTNodeResult::Succeeded;
}