// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Service/BTService_EnemyEyeSight.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/AIController/EnemyBaseAIController.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Character/ArcanePunkCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "AnimInstance/AP_EnemyBaseAnimInstance.h"
#include "GameMode/APGameModeBase.h"

UBTService_EnemyEyeSight::UBTService_EnemyEyeSight()
{
    NodeName = TEXT("Enemy EyeSight");
}

void UBTService_EnemyEyeSight::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    if(!OwnerComp.GetAIOwner()) return;

    AEnemyBaseAIController* AIController = Cast<AEnemyBaseAIController>(OwnerComp.GetAIOwner());
    if(!AIController) return;

    AEnemy_CharacterBase* Monster = Cast<AEnemy_CharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
    if(!Monster) return;
     auto Anim = Cast<UAP_EnemyBaseAnimInstance>(Monster->GetMesh()->GetAnimInstance()); if(!Anim) return; 

    TWeakObjectPtr<AActor> TargetActor = Monster->IsAggro();
    if(!TargetActor.IsValid())
    {
        TargetActor = GetPlayerActor(Monster);
        if(AArcanePunkCharacter* TargetPlayer = Cast<AArcanePunkCharacter>(TargetActor.Get())) 
        {
            if(!TargetPlayer->GetHideMode()){TargetActor = TargetPlayer;}
            else {TargetActor = nullptr;}
        }
    }   

    if(TargetActor.IsValid())
    {
        FVector TargetLocation = TargetActor->GetActorLocation();
        float Distance =  FVector::Distance(Monster->GetActorLocation(), TargetLocation);
        
        if((AIController->GetEyeSightTrace() ? AIController->LineOfSightTo(TargetActor.Get()) : true) && (AIController->GetDistanceTrace() ? Distance < Monster->GetDetectDist() : true))
        {
            OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), TargetActor.Get());
            
            if(BattleLoopCondition) IsLoop = true;
        }
        else
        {
            if(IsLoop)
            {
                return;
            }
            else
            {
                OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
                Anim->SetbBattleMode(false);
            }
        }
    }
    else
    {
        OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
        Anim->SetbBattleMode(false);
    }

}

AActor *UBTService_EnemyEyeSight::GetPlayerActor(AActor* AIOwner)
{
    // TArray<AActor*> Actors;
    // UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Player"), Actors);

    // if(Actors.IsEmpty())
    // {
    //     APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    //     if(!PlayerPawn) return nullptr;
    //     Actors.Emplace(PlayerPawn);
    // }

    // if(Actors.Num() >= 2)
	// {
	// 	Actors.Sort([AIOwner](AActor& A, AActor& B) 
	// 	{
	// 		return (A.GetActorLocation() - AIOwner->GetActorLocation()).Size() > (B.GetActorLocation() - AIOwner->GetActorLocation()).Size();
	// 	});
	// }

    // return Actors.Top();
    auto GM = Cast<AAPGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); if(!GM) return;
    return GM->GetMainPlayer();
}
