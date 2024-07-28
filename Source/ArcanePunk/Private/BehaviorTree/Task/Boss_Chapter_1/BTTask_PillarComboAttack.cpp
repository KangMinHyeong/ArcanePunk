// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/Boss_Chapter_1/BTTask_PillarComboAttack.h"

#include "AIController.h"
#include "Enemy/Enemy_Boss.h"

UBTTask_PillarComboAttack::UBTTask_PillarComboAttack()
{
    NodeName = TEXT("Pillar Combo Attack");
}

EBTNodeResult::Type UBTTask_PillarComboAttack::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    if(!OwnerComp.GetAIOwner()) return EBTNodeResult::Failed;


    AEnemy_Boss* Boss = Cast<AEnemy_Boss>(OwnerComp.GetAIOwner()->GetPawn());
    if(!Boss) return EBTNodeResult::Failed;
    
    Boss->OnPillarCombo();
    
    return EBTNodeResult::Succeeded;
}