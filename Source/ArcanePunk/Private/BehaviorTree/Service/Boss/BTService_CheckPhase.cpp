
#include "BehaviorTree/Service/Boss/BTService_CheckPhase.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/AIController/EnemyBaseAIController.h"
#include "Enemy/Enemy_BossBase.h"

UBTService_CheckPhase::UBTService_CheckPhase()
{
    NodeName = TEXT("Check Phase");
}

void UBTService_CheckPhase::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    if(OwnerComp.GetAIOwner() == nullptr) return;

    auto Boss = Cast<AEnemy_BossBase>(OwnerComp.GetAIOwner()->GetPawn());
    if(!Boss) return;

    OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), (uint8)Boss->GetPhase());
}