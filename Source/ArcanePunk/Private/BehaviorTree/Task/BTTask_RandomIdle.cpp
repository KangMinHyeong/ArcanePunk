
#include "BehaviorTree/Task/BTTask_RandomIdle.h"

#include "AIController.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AnimInstance/AP_EnemyBaseAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_RandomIdle::UBTTask_RandomIdle()
{
    NodeName = TEXT("Random Idle");
}

EBTNodeResult::Type UBTTask_RandomIdle::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    if(!OwnerComp.GetAIOwner()) return EBTNodeResult::Failed;

    AEnemy_CharacterBase* Monster = Cast<AEnemy_CharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
    if(!Monster) return EBTNodeResult::Failed;

    auto Anim = Cast<UAP_EnemyBaseAnimInstance>(Monster->GetMesh()->GetAnimInstance());
    if(!Anim) return EBTNodeResult::Failed;

    Anim->PlayRandomIdle_Montage();

    return EBTNodeResult::Succeeded;
}
