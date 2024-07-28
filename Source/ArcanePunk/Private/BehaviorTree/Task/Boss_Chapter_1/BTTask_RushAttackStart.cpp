// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/Boss_Chapter_1/BTTask_RushAttackStart.h"

#include "AIController.h"
#include "Enemy/Enemy_Boss.h"

UBTTask_RushAttackStart::UBTTask_RushAttackStart()
{
    NodeName = TEXT("Rush Attack Start");
}

EBTNodeResult::Type UBTTask_RushAttackStart::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    if(!OwnerComp.GetAIOwner()) return EBTNodeResult::Failed;


    AEnemy_Boss* Boss = Cast<AEnemy_Boss>(OwnerComp.GetAIOwner()->GetPawn());
    if(!Boss) return EBTNodeResult::Failed;
    
    Boss->RushAttackStart();

    return EBTNodeResult::Succeeded;
}
