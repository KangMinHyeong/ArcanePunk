
#include "BehaviorTree/Task/BTTask_PillarAttackEnd.h"

#include "AIController.h"
#include "Enemy/Enemy_Boss.h"

UBTTask_PillarAttackEnd::UBTTask_PillarAttackEnd()
{
    NodeName = TEXT("Pillar Attack End");
}

EBTNodeResult::Type UBTTask_PillarAttackEnd::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    if(!OwnerComp.GetAIOwner()) return EBTNodeResult::Failed;


    AEnemy_Boss* Boss = Cast<AEnemy_Boss>(OwnerComp.GetAIOwner()->GetPawn());
    if(!Boss) return EBTNodeResult::Failed;
    
    Boss->OnPillarAttackEnd();

    return EBTNodeResult::Succeeded;
}
