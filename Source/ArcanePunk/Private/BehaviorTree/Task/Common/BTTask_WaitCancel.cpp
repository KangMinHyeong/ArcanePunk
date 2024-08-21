
#include "BehaviorTree/Task/Common/BTTask_WaitCancel.h"

#include "AIController.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "AnimInstance/AP_EnemyBaseAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"

UBTTask_WaitCancel::UBTTask_WaitCancel()
{
    NodeName = TEXT("Wait Cancel");
}

EBTNodeResult::Type UBTTask_WaitCancel::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{    
    Super::ExecuteTask(OwnerComp, NodeMemory);

    SetAIOwner(OwnerComp);
    if(!Enemy.IsValid()) return EBTNodeResult::Failed;
	if(!EnemyAnim.IsValid()) return EBTNodeResult::Failed;

    Enemy->GetMesh()->SetCanEverAffectNavigation(false);

    return EBTNodeResult::InProgress;
}