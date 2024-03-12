
#include "NPC/AIController/APNPCAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NPC/APNPCBase.h"

void AAPNPCAIController::BeginPlay()
{
    Super::BeginPlay();
}

void AAPNPCAIController::OnPossess(APawn *InPawn)
{
    Super::OnPossess(InPawn);

    auto NPC = Cast<AAPNPCBase>(GetPawn()); if(!NPC) return;

    if(AIBehavior != nullptr)
    {   
        RunBehaviorTree(AIBehavior);
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), NPC->GetActorLocation());
        GetBlackboardComponent()->SetValueAsVector(TEXT("Destination"), NPC->GetDestination());
        GetBlackboardComponent()->SetValueAsVector(TEXT("WayPoint"), NPC->GetDestination());
        GetBlackboardComponent()->SetValueAsBool(TEXT("Movement"), NPC->IsMovement());
        
    }
}
