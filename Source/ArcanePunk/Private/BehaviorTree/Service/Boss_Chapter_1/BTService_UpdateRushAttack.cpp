// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Service/Boss_Chapter_1/BTService_UpdateRushAttack.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Enemy/Enemy_Boss.h"

UBTService_UpdateRushAttack::UBTService_UpdateRushAttack()
{
    NodeName = TEXT("Update RushAttack");
}

void UBTService_UpdateRushAttack::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    if(!OwnerComp.GetAIOwner()) return;

    AEnemy_Boss* Boss = Cast<AEnemy_Boss>(OwnerComp.GetAIOwner()->GetPawn());
    if(!Boss) return;

    OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), Boss->GetRushAttack());
}