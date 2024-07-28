
#include "BehaviorTree/Task/Boss_Chapter_1/BTTask_KnockBackAttackReady.h"

#include "AIController.h"
#include "Enemy/Enemy_Boss.h"

UBTTask_KnockBackAttackReady::UBTTask_KnockBackAttackReady()
{
    NodeName = TEXT("KnockBack Attack Ready");
}

EBTNodeResult::Type UBTTask_KnockBackAttackReady::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    if(!OwnerComp.GetAIOwner()) return EBTNodeResult::Failed;

    AEnemy_Boss* Boss = Cast<AEnemy_Boss>(OwnerComp.GetAIOwner()->GetPawn());
    if(!Boss) return EBTNodeResult::Failed;
    
    Boss->KnockBackAttack();

    return EBTNodeResult::Succeeded;
}