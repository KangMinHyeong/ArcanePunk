
#include "BehaviorTree/Task/Common/BTTask_Detect.h"

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
    SetAIOwner(OwnerComp);
    if(!Enemy.IsValid()) return EBTNodeResult::Failed;
	if(!EnemyAnim.IsValid()) return EBTNodeResult::Failed;

    WaitTime = 0.0f;
    if(!EnemyAnim->IsBattleMode())
    {
        EnemyAnim->SetbBattleMode(true);
        EnemyAnim->PlayDetect_Montage();
        Enemy->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
        Enemy->GetCharacterMovement()->MaxWalkSpeed = Enemy->GetDefaultSpeed_Origin();
        Enemy->GetCharacterMovement()->MaxFlySpeed = Enemy->GetDefaultSpeed_Origin();
        Enemy->SpawnDetectRender();

        CalculateWaitTime_Montage(EnemyAnim->Detect_Montage);
    }
    
    Enemy->GetMesh()->SetCanEverAffectNavigation(false);
    Super::ExecuteTask(OwnerComp, NodeMemory);

    return EBTNodeResult::InProgress;
}
