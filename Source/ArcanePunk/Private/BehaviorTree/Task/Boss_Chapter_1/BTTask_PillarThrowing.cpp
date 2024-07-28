
#include "BehaviorTree/Task/Boss_Chapter_1/BTTask_PillarThrowing.h"

#include "AIController.h"
#include "Enemy/Enemy_Boss.h"
#include "Character/ArcanePunkCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_PillarThrowing::UBTTask_PillarThrowing()
{
    NodeName = TEXT("Pillar Throwing");
}

EBTNodeResult::Type UBTTask_PillarThrowing::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    if(!OwnerComp.GetAIOwner()) return EBTNodeResult::Failed;


    AEnemy_Boss* Boss = Cast<AEnemy_Boss>(OwnerComp.GetAIOwner()->GetPawn());
    if(!Boss) return EBTNodeResult::Failed;

    auto Player = Cast<AArcanePunkCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
    if (!Player) return EBTNodeResult::Failed;

    Boss->OnThrowingPillarReady(Player->GetActorLocation() - Boss->GetActorLocation());
    
    return EBTNodeResult::Succeeded;
}
