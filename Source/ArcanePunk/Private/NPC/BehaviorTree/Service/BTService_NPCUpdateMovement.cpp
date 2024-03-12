
#include "NPC/BehaviorTree/Service/BTService_NPCUpdateMovement.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NPC/APNPCBase.h"

UBTService_NPCUpdateMovement::UBTService_NPCUpdateMovement()
{
    NodeName = TEXT("NPC Update Movement");
}

void UBTService_NPCUpdateMovement::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


    auto NPC = Cast<AAPNPCBase>(OwnerComp.GetAIOwner()->GetPawn());
    if(!NPC) return;

    OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), NPC->IsMovement());
}