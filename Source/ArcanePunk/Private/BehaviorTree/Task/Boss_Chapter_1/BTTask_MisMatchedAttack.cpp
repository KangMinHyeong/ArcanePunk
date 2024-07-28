
#include "BehaviorTree/Task/Boss_Chapter_1/BTTask_MisMatchedAttack.h"

#include "AIController.h"
#include "Enemy/Enemy_Boss.h"
#include "AnimInstance/AP_EnemyBossAnimInstance.h"

UBTTask_MisMatchedAttack::UBTTask_MisMatchedAttack()
{
    NodeName = TEXT("MisMatched Attack");
}

EBTNodeResult::Type UBTTask_MisMatchedAttack::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    if(!OwnerComp.GetAIOwner()) return EBTNodeResult::Failed;

    AEnemy_Boss* Boss = Cast<AEnemy_Boss>(OwnerComp.GetAIOwner()->GetPawn());
    if(!Boss) return EBTNodeResult::Failed;
    
    auto BossAnim = Cast<UAP_EnemyBossAnimInstance>(Boss->GetMesh()->GetAnimInstance());
    if(BossAnim) BossAnim->PlayMismatchedAttack_Montage();

    return EBTNodeResult::Succeeded;
}
