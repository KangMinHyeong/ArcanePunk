
#include "BehaviorTree/Decorator/BTDecorator_SelectPatrol.h"

#include "AIController.h"
#include "Enemy/Enemy_CharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AnimInstance/AP_EnemyBaseAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTDecorator_SelectPatrol::UBTDecorator_SelectPatrol()
{
    NodeName = TEXT("Select Patrol");    
}

bool UBTDecorator_SelectPatrol::CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const
{
    bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    auto Monster = Cast<AEnemy_CharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Monster) return false;

    OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Monster->GetPatrolLocation(OwnerComp.GetBlackboardComponent()->GetValueAsVector(TEXT("StartLocation"))));

    Monster->GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
    
	return true;
}
