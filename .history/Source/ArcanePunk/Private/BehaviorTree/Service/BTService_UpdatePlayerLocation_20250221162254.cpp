// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Service/BTService_UpdatePlayerLocation.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/AIController/EnemyBaseAIController.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "PlayerController/ArcanePunkPlayerController.h"

UBTService_UpdatePlayerLocation::UBTService_UpdatePlayerLocation()
{
    NodeName = TEXT("Update PlayerLocation");
}

void UBTService_UpdatePlayerLocation::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    auto PC = Cast<AArcanePunkPlayerController>(GetWorld()->GetFirstPlayerController()); if(!PC) return;
    APawn* PlayerPawn = PC->GetMainPlayer();

    AEnemyBaseAIController* AIController = Cast<AEnemyBaseAIController>(OwnerComp.GetAIOwner());
    if(!AIController) return;

    AEnemy_CharacterBase* Monster = Cast<AEnemy_CharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
    if(!Monster) return;

    AActor* TargetActor = Monster->IsAggro();
    if(!Monster->IsAggro())
    {
        TargetActor = PlayerPawn;
    } 

    OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), TargetActor->GetActorLocation());
}