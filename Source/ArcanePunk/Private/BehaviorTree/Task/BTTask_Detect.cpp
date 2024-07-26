
#include "BehaviorTree/Task/BTTask_Detect.h"

#include "AIController.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AnimInstance/AP_EnemyBaseAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_Detect::UBTTask_Detect()
{
    NodeName = TEXT("Detect");
}

EBTNodeResult::Type UBTTask_Detect::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    if(!OwnerComp.GetAIOwner()) return EBTNodeResult::Failed;

    AEnemy_CharacterBase* Monster = Cast<AEnemy_CharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
    if(!Monster) return EBTNodeResult::Failed;

    auto Anim = Cast<UAP_EnemyBaseAnimInstance>(Monster->GetMesh()->GetAnimInstance());
    if(!Anim) return EBTNodeResult::Failed;

    if(!Anim->IsBattleMode())
    {
        Anim->SetbBattleMode(true);
        Anim->PlayDetect_Montage();
        Monster->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
        Monster->GetCharacterMovement()->MaxWalkSpeed = Monster->GetDefaultSpeed_Origin();
        Monster->SpawnDetectRender();
    }
    
    return EBTNodeResult::Succeeded;
}
