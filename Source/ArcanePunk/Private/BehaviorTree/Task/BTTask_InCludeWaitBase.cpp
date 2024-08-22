

#include "BehaviorTree/Task/BTTask_InCludeWaitBase.h"

#include "AIController.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "Enemy/Enemy_BossBase.h"
#include "AnimInstance/AP_EnemyBaseAnimInstance.h"

void UBTTask_InCludeWaitBase::InitializeFromAsset(UBehaviorTree &Asset)
{
    Super::InitializeFromAsset(Asset);

    DelayTime = WaitTime;
}

void UBTTask_InCludeWaitBase::SetAIOwner(UBehaviorTreeComponent &OwnerComp)
{
    if(!OwnerComp.GetAIOwner()) return;

    Enemy = Cast<AEnemy_CharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
    if(!Enemy.IsValid()) return;
    
    EnemyAnim = Cast<UAP_EnemyBaseAnimInstance>(Enemy->GetMesh()->GetAnimInstance());
	if(!EnemyAnim.IsValid()) return;
}

void UBTTask_InCludeWaitBase::SetAIOwner_Boss(UBehaviorTreeComponent &OwnerComp)
{
    if(!OwnerComp.GetAIOwner()) return;

    Boss = Cast<AEnemy_BossBase>(OwnerComp.GetAIOwner()->GetPawn());
    if(!Boss.IsValid()) return;
    
    EnemyAnim = Cast<UAP_EnemyBaseAnimInstance>(Boss->GetMesh()->GetAnimInstance());
	if(!EnemyAnim.IsValid()) return;
}

void UBTTask_InCludeWaitBase::CalculateWaitTime_Montage(float AnimSequenceTime)
{
    WaitTime = DelayTime + AnimSequenceTime + FMath::RandRange(-RandomDeviation, RandomDeviation);
    WaitTime = FMath::Max(0.0f, WaitTime);
}
