
#include "BehaviorTree/Task/BTTask_RangeAttack2.h"

#include "AIController.h"
#include "Enemy/Enemy_Boss.h"

UBTTask_RangeAttack2::UBTTask_RangeAttack2()
{
    NodeName = TEXT("Range Attack2");
}

EBTNodeResult::Type UBTTask_RangeAttack2::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    if(!OwnerComp.GetAIOwner()) return EBTNodeResult::Failed;


    AEnemy_Boss* Boss = Cast<AEnemy_Boss>(OwnerComp.GetAIOwner()->GetPawn());
    if(!Boss) return EBTNodeResult::Failed;
    
    Boss->RangeAttack_2_Ready();

    return EBTNodeResult::Succeeded;
}