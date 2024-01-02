// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Service/BTService_UpdatePlayerLocation.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"

UBTService_UpdatePlayerLocation::UBTService_UpdatePlayerLocation()
{
    NodeName = TEXT("Update PlayerLocation");
}

void UBTService_UpdatePlayerLocation::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if(PlayerPawn == nullptr) return;

    OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerPawn->GetActorLocation());
}