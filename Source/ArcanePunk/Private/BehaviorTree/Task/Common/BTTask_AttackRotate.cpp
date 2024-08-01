
#include "BehaviorTree/Task/Common/BTTask_AttackRotate.h"

#include "AIController.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_AttackRotate::UBTTask_AttackRotate()
{
    NodeName = TEXT("Attack Rotate");
}

EBTNodeResult::Type UBTTask_AttackRotate::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    if(!OwnerComp.GetAIOwner())
    {
        return EBTNodeResult::Failed;
    }

    AEnemy_CharacterBase* Monster = Cast<AEnemy_CharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
    if(!Monster)
    {   
        return EBTNodeResult::Failed;
    }

    Monster->GetMesh()->SetCanEverAffectNavigation(true);

    auto Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BlackboardKey.SelectedKeyName));
    if(Target) Monster->RotateTowardsTarget(Target, RotateSpeed);

    return EBTNodeResult::InProgress;
}
