
#include "BehaviorTree/Task/Common/BTTask_RandomPlayNumber.h"

#include "AIController.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_RandomPlayNumber::UBTTask_RandomPlayNumber()
{
    NodeName = TEXT("Random PlayNumber");
}

EBTNodeResult::Type UBTTask_RandomPlayNumber::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if(!OwnerComp.GetAIOwner()) return EBTNodeResult::Failed;

    auto Enemy = Cast<AEnemy_CharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
    if(!Enemy) return EBTNodeResult::Failed;

    Enemy->GetMesh()->SetCanEverAffectNavigation(true);

    int32 Current = FMath::RandRange(0, MaxNumber);
    OwnerComp.GetBlackboardComponent()->SetValueAsInt(BlackboardKey.SelectedKeyName, Current);

    return EBTNodeResult::InProgress;
}
