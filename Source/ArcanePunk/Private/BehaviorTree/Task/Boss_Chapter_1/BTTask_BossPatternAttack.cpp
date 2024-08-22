// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/Boss_Chapter_1/BTTask_BossPatternAttack.h"

#include "AIController.h"
#include "Enemy/Enemy_BossBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AnimInstance/AP_EnemyBaseAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_BossPatternAttack::UBTTask_BossPatternAttack()
{
    NodeName = TEXT("Boss Pattern Attacks");
}

EBTNodeResult::Type UBTTask_BossPatternAttack::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    SetAIOwner_Boss(OwnerComp);
    if(!Boss.IsValid()) return EBTNodeResult::Failed;
	if(!EnemyAnim.IsValid()) return EBTNodeResult::Failed;
    
    auto phase = OwnerComp.GetBlackboardComponent()->GetValueAsInt(CurrentPhase.SelectedKeyName);
    auto pattern = OwnerComp.GetBlackboardComponent()->GetValueAsInt(BlackboardKey.SelectedKeyName);

    if(phase == 1)
    {
        CalculateWaitTime_Montage(EnemyAnim->Play_Phase1Attack_Montage(pattern));
    }
    else if(phase == 2)
    {
        CalculateWaitTime_Montage(EnemyAnim->Play_Phase2Attack_Montage(pattern));
    }
    
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    // Enemy->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

    return EBTNodeResult::InProgress;
}
