// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTService_IsHitting.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/EnemyBaseAIController.h"
#include "Enemy/Enemy_CharacterBase.h"

UBTService_IsHitting::UBTService_IsHitting()
{
    NodeName = TEXT("IsHitting");
}

void UBTService_IsHitting::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    if(OwnerComp.GetAIOwner() == nullptr) return;

    AEnemyBaseAIController* AIController = Cast<AEnemyBaseAIController>(OwnerComp.GetAIOwner());
    if(!AIController) return;

    AEnemy_CharacterBase* Monster = Cast<AEnemy_CharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
    if(!Monster) return;

    OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), Monster->IsHitting());
}
