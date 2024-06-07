
#include "BehaviorTree/Task/BTTask_PillarAttackReady.h"

#include "AIController.h"
#include "Enemy/Enemy_Boss.h"

UBTTask_PillarAttackReady::UBTTask_PillarAttackReady()
{
    NodeName = TEXT("PillarAttack Ready");
}

EBTNodeResult::Type UBTTask_PillarAttackReady::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    if(!OwnerComp.GetAIOwner()) return EBTNodeResult::Failed;


    AEnemy_Boss* Boss = Cast<AEnemy_Boss>(OwnerComp.GetAIOwner()->GetPawn());
    if(!Boss) return EBTNodeResult::Failed;
    
    if(!Boss->HasPillar()) return EBTNodeResult::Failed;

    return EBTNodeResult::Succeeded;
}