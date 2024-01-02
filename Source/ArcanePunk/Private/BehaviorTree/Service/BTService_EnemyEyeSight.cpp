// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Service/BTService_EnemyEyeSight.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/AIController/EnemyBaseAIController.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Character/ArcanePunkCharacter.h"
#include "Kismet/GameplayStatics.h"

UBTService_EnemyEyeSight::UBTService_EnemyEyeSight()
{
    NodeName = TEXT("Enemy EyeSight");
}

void UBTService_EnemyEyeSight::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if(OwnerComp.GetAIOwner() == nullptr) return;

    AEnemyBaseAIController* AIController = Cast<AEnemyBaseAIController>(OwnerComp.GetAIOwner());
    if(!AIController) return;

    AEnemy_CharacterBase* Monster = Cast<AEnemy_CharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
    if(!Monster) return;

    AArcanePunkCharacter* TargetPlayer = Cast<AArcanePunkCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
    if(!TargetPlayer) return;

    FVector TargetLocation = TargetPlayer->GetActorLocation();
    float Distance =  FVector::Distance(Monster->GetActorLocation(), TargetLocation);

    if((AIController->GetEyeSightTrace() ? AIController->LineOfSightTo(TargetPlayer) : true) && (AIController->GetDistanceTrace() ? Distance < Monster->GetDistanceLimit() : true))
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), TargetPlayer);
    }
    else
    {
        OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
    }
}
