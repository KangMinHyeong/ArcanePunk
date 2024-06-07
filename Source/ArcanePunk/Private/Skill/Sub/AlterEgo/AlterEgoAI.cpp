
#include "Skill/Sub/AlterEgo/AlterEgoAI.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Skill/ArcaneAlterEgo.h"

void AAlterEgoAI::OnPossess(APawn *InPawn)
{
    Super::OnPossess(InPawn);

    auto Ego = Cast<AArcaneAlterEgo>(GetPawn()); if(!Ego) return;

    if(AIBehavior != nullptr)
    {   
        RunBehaviorTree(AIBehavior);
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), Ego->GetActorLocation());
        // GetBlackboardComponent()->SetValueAsVector(TEXT("Destination"), Ego->GetDestination());
        // GetBlackboardComponent()->SetValueAsVector(TEXT("WayPoint"), Ego->GetDestination());
        // GetBlackboardComponent()->SetValueAsBool(TEXT("Movement"), Ego->IsMovement());
    }
}
