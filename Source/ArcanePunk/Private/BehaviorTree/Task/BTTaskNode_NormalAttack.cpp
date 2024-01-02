// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/BTTaskNode_NormalAttack.h"

#include "AIController.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "AnimInstance/AP_EnemyBaseAnimInstance.h"

UBTTaskNode_NormalAttack::UBTTaskNode_NormalAttack()
{
    NodeName = TEXT("Normal Attack");
}

EBTNodeResult::Type UBTTaskNode_NormalAttack::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    if(!OwnerComp.GetAIOwner())
    {
        return EBTNodeResult::Failed;
    }

    AEnemy_CharacterBase* Monster = Cast<AEnemy_CharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
    if(!Monster)
    {   
        return EBTNodeResult::Failed;
    }

    auto EnemyAnim = Cast<UAP_EnemyBaseAnimInstance>(Monster->GetMesh()->GetAnimInstance());
	if(EnemyAnim) EnemyAnim->PlayNormalAttack_Montage();

    return EBTNodeResult::Succeeded;
}
