#include "BehaviorTree/Task/BTTask_RangeAttack1.h"

#include "AIController.h"
#include "Enemy/Enemy_Boss.h"

UBTTask_RangeAttack1::UBTTask_RangeAttack1()
{
    NodeName = TEXT("Range Attack1");
}

EBTNodeResult::Type UBTTask_RangeAttack1::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    if(!OwnerComp.GetAIOwner()) return EBTNodeResult::Failed;


    AEnemy_Boss* Boss = Cast<AEnemy_Boss>(OwnerComp.GetAIOwner()->GetPawn());
    if(!Boss) return EBTNodeResult::Failed;
    
    Boss->RangeAttack_1_Ready();

    return EBTNodeResult::Succeeded;
}
