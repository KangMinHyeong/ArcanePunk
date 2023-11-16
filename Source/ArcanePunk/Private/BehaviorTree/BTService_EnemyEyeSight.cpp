// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTService_EnemyEyeSight.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTService_EnemyEyeSight::UBTService_EnemyEyeSight()
{
    NodeName = TEXT("Enemy EyeSight");
}

void UBTService_EnemyEyeSight::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if(PlayerPawn == nullptr) return;
    if(OwnerComp.GetAIOwner() == nullptr) return;

    AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());

    if(AIController == nullptr) return;

    if (AIController->LineOfSightTo(PlayerPawn))
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), PlayerPawn);
    }
    else
    {
        OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
    }
}
