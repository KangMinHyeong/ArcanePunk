// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AIController/EnemyBaseAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

void AEnemyBaseAIController::BeginPlay()
{
    Super::BeginPlay();
}

void AEnemyBaseAIController::OnPossess(APawn *InPawn)
{
    Super::OnPossess(InPawn);

    if(AIBehavior != nullptr)
    {   
        RunBehaviorTree(AIBehavior);
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
    }
}

bool AEnemyBaseAIController::GetEyeSightTrace()
{
    return EyeSightTrace;
}

bool AEnemyBaseAIController::GetDistanceTrace()
{
    return DistanceTrace;
}

void AEnemyBaseAIController::UnPossessing()
{
    OnUnPossess();
}
