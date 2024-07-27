
#include "BehaviorTree/Task/BTTask_RandomPlayNumber.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_RandomPlayNumber::UBTTask_RandomPlayNumber()
{
    NodeName = TEXT("Random PlayNumber");
}

EBTNodeResult::Type UBTTask_RandomPlayNumber::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    int32 Current = FMath::RandRange(0, MaxNumber);
    OwnerComp.GetBlackboardComponent()->SetValueAsInt(BlackboardKey.SelectedKeyName, Current);

    return EBTNodeResult::InProgress;
}
