// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Service/BTService_EnemyEyeSight.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/AIController/EnemyBaseAIController.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Character/ArcanePunkCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "AnimInstance/AP_EnemyBaseAnimInstance.h"

UBTService_EnemyEyeSight::UBTService_EnemyEyeSight()
{
    NodeName = TEXT("Enemy EyeSight");
}

void UBTService_EnemyEyeSight::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

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

        if((AIController->GetEyeSightTrace() ? AIController->LineOfSightTo(TargetActor.Get()) : true) && (AIController->GetDistanceTrace() ? Distance < Monster->GetDistanceLimit() : true))
        {
            OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), TargetActor.Get());
            
            if(BattleLoopCondition) IsLoop = true;
        }
        else
        {
            if(IsLoop)
            {
                OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), TargetActor.Get());
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
    // Cast<AArcanePunkCharacter>(UGameplayStatics::GetPlayerPawn(this, 0)); 
    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Player"), Actors);

    if(Actors.IsEmpty()) return nullptr;

    if(Actors.Num() >= 2)
	{
		Actors.Sort([AIOwner](AActor& A, AActor& B) 
		{
			return (A.GetActorLocation() - AIOwner->GetActorLocation()).Size() > (B.GetActorLocation() - AIOwner->GetActorLocation()).Size();
		});
	}

    return Actors.Top();
}
