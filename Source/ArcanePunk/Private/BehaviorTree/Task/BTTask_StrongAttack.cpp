
#include "BehaviorTree/Task/BTTask_StrongAttack.h"

#include "AIController.h"
#include "Enemy/Enemy_Boss.h"
#include "AnimInstance/AP_EnemyBossAnimInstance.h"

UBTTask_StrongAttack::UBTTask_StrongAttack()
{
    NodeName = TEXT("Strong Attack");
}

EBTNodeResult::Type UBTTask_StrongAttack::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    if(!OwnerComp.GetAIOwner()) return EBTNodeResult::Failed;

    AEnemy_Boss* Boss = Cast<AEnemy_Boss>(OwnerComp.GetAIOwner()->GetPawn());
    if(!Boss) return EBTNodeResult::Failed;
    
    auto BossAnim = Cast<UAP_EnemyBossAnimInstance>(Boss->GetMesh()->GetAnimInstance());
    if(BossAnim) BossAnim->PlayStrongAttack_Montage();

    return EBTNodeResult::Succeeded;
}
