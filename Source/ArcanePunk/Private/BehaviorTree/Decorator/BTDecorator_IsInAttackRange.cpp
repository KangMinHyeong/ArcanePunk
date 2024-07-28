
#include "BehaviorTree/Decorator/BTDecorator_IsInAttackRange.h"

#include "AIController.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AnimInstance/AP_EnemyBaseAnimInstance.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
    NodeName = TEXT("In Attack Range");    
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const
{
    bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    auto Monster = Cast<AEnemy_CharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Monster) return false;

    auto Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
    if(!Player) return false;

    if((Player->GetActorLocation() - Monster->GetActorLocation()).Size() > AttackRange) return true;

	return false;
}