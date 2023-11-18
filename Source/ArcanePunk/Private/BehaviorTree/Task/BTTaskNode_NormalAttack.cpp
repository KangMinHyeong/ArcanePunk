// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/BTTaskNode_NormalAttack.h"

#include "AIController.h"
#include "Enemy/Enemy_CharacterBase.h"

UBTTaskNode_NormalAttack::UBTTaskNode_NormalAttack()
{
    NodeName = TEXT("Normal Attack");
}

EBTNodeResult::Type UBTTaskNode_NormalAttack::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    if(OwnerComp.GetAIOwner() == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    AEnemy_CharacterBase* Monster = Cast<AEnemy_CharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
    if(Monster == nullptr)
    {   
        return EBTNodeResult::Failed;
    }

    Monster->NormalAttack();

    return EBTNodeResult::Succeeded;
}
