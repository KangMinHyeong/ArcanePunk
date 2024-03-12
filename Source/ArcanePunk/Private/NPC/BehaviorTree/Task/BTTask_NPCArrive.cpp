
#include "NPC/BehaviorTree/Task/BTTask_NPCArrive.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "NPC/APNPCBase.h"
#include "AIController.h"

UBTTask_NPCArrive::UBTTask_NPCArrive()
{
    NodeName = TEXT("NPC Arrive");
}

EBTNodeResult::Type UBTTask_NPCArrive::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    auto NPC = Cast<AAPNPCBase>(OwnerComp.GetAIOwner()->GetPawn()); if(!NPC) return EBTNodeResult::Failed;

    NPC->InteractionCollisionEnable(true);

    if(NPC->IsStart())
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("WayPoint"), OwnerComp.GetBlackboardComponent()->GetValueAsVector(TEXT("StartLocation")));
    }
    else
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("WayPoint"), OwnerComp.GetBlackboardComponent()->GetValueAsVector(TEXT("Destination")));
    }
    NPC->SetStart(!NPC->IsStart());

    OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), NPC->ChangeMovement(OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey())));

    return EBTNodeResult::Succeeded;
}
