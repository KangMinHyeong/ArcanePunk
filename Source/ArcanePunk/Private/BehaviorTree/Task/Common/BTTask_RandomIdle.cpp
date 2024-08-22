
#include "BehaviorTree/Task/Common/BTTask_RandomIdle.h"

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
    SetAIOwner(OwnerComp);
    if(!Enemy.IsValid()) return EBTNodeResult::Failed;
	if(!EnemyAnim.IsValid()) return EBTNodeResult::Failed;  

    auto Current = OwnerComp.GetBlackboardComponent()->GetValueAsInt(BlackboardKey.SelectedKeyName) - 1;
    OwnerComp.GetBlackboardComponent()->SetValueAsInt(BlackboardKey.SelectedKeyName, Current);

    CalculateWaitTime_Montage(EnemyAnim->PlayRandomIdle_Montage());
    
    Super::ExecuteTask(OwnerComp, NodeMemory);

    return EBTNodeResult::InProgress;
}
