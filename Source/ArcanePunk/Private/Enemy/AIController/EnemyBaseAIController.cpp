// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AIController/EnemyBaseAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "ArcanePunk/Public/Character/ArcanePunkCharacter.h"
#include "Enemy/Enemy_CharacterBase.h"

AEnemyBaseAIController::AEnemyBaseAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent"))) 
{
    CrowdFollowComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent());
    CrowdFollowComp->SetCrowdSimulationState(ECrowdSimulationState::Enabled);
    CrowdFollowComp->SetCrowdPathOffset(true);
    CrowdFollowComp->SetCrowdAnticipateTurns(true);
	CrowdFollowComp->SetCrowdObstacleAvoidance(true);
	CrowdFollowComp->SetCrowdSeparation(true);
	
	CrowdFollowComp->SetCrowdAffectFallingVelocity(false);
	CrowdFollowComp->SetCrowdRotateToVelocity(false);

    CrowdFollowComp->SetCrowdCollisionQueryRange(100.0f);
	CrowdFollowComp->SetCrowdPathOptimizationRange(1000.0f);

    CrowdFollowComp->SetCrowdSeparationWeight(1.0f);
    CrowdFollowComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);

}

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
        if(GetBlackboardComponent())
        {
            GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
        }


        // UE_LOG(LogTemp, Display, TEXT("GetCrowdAgentLocation %f"), CrowdFollowComp->GetCrowdAgentLocation());
        // if(CrowdFollowComp->IsCrowdSimulationEnabled()) UE_LOG(LogTemp, Display, TEXT("Your message"));
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

