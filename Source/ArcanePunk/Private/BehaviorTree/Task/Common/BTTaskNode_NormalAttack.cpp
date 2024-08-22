// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/Common/BTTaskNode_NormalAttack.h"

#include "AIController.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AnimInstance/AP_EnemyBaseAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTaskNode_NormalAttack::UBTTaskNode_NormalAttack()
{
    NodeName = TEXT("Normal Attack");
}

EBTNodeResult::Type UBTTaskNode_NormalAttack::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{   
    SetAIOwner(OwnerComp);
    if(!Enemy.IsValid()) return EBTNodeResult::Failed;
	if(!EnemyAnim.IsValid()) return EBTNodeResult::Failed;
  
    
    CalculateWaitTime_Montage(EnemyAnim->PlayNormalAttack_Montage());
    Enemy->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

    Super::ExecuteTask(OwnerComp, NodeMemory);
    return EBTNodeResult::InProgress;
}
