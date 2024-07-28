
#include "BehaviorTree/Task/BTTask_MoveToCheckDist.h"

#include "AIController.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AnimInstance/AP_EnemyBaseAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_MoveToCheckDist::UBTTask_MoveToCheckDist()
{
    NodeName = TEXT("MoveTo CheckDist");
}

EBTNodeResult::Type UBTTask_MoveToCheckDist::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{    
    if(!OwnerComp.GetAIOwner()) return EBTNodeResult::Failed;

    auto Enemy = Cast<AEnemy_CharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
    if(!Enemy) return EBTNodeResult::Failed;
    auto Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
    if(!Player) return EBTNodeResult::Failed;

    AcceptableRadius = Enemy->GetMonsterAttackRange();

    if((Player->GetActorLocation() - Enemy->GetActorLocation()).Size() <= AcceptableRadius) 
    return EBTNodeResult::Succeeded;

    return Super::ExecuteTask(OwnerComp, NodeMemory);
}
