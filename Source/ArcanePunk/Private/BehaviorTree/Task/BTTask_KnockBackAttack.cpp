
#include "BehaviorTree/Task/BTTask_KnockBackAttack.h"

#include "AIController.h"
#include "Enemy/Enemy_Boss.h"

UBTTask_KnockBackAttack::UBTTask_KnockBackAttack()
{
    NodeName = TEXT("KnockBack Attack");
}

EBTNodeResult::Type UBTTask_KnockBackAttack::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    if(!OwnerComp.GetAIOwner()) return EBTNodeResult::Failed;

    AEnemy_Boss* Boss = Cast<AEnemy_Boss>(OwnerComp.GetAIOwner()->GetPawn());
    if(!Boss) return EBTNodeResult::Failed;
    
    Boss->KnockBackAttackReady();

    return EBTNodeResult::Succeeded;
}