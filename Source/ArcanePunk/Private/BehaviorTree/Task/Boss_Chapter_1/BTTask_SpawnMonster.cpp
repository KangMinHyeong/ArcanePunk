#include "BehaviorTree/Task/Boss_Chapter_1/BTTask_SpawnMonster.h"

#include "AIController.h"
#include "Enemy/Enemy_Boss.h"
#include "AnimInstance/AP_EnemyBossAnimInstance.h"

UBTTask_SpawnMonster::UBTTask_SpawnMonster()
{
    NodeName = TEXT("Spawn Monster");
}

EBTNodeResult::Type UBTTask_SpawnMonster::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    if(!OwnerComp.GetAIOwner()) return EBTNodeResult::Failed;

    AEnemy_Boss* Boss = Cast<AEnemy_Boss>(OwnerComp.GetAIOwner()->GetPawn());
    if(!Boss) return EBTNodeResult::Failed;
    
    auto BossAnim = Cast<UAP_EnemyBossAnimInstance>(Boss->GetMesh()->GetAnimInstance());
    if(BossAnim) BossAnim->PlaySpawnMonster_Montage();

    return EBTNodeResult::Succeeded;
}