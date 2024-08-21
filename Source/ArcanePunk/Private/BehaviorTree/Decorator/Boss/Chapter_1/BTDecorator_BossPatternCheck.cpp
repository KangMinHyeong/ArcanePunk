
#include "BehaviorTree/Decorator/Boss/Chapter_1/BTDecorator_BossPatternCheck.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Enemy/Enemy_BossBase.h"

UBTDecorator_BossPatternCheck::UBTDecorator_BossPatternCheck()
{
    NodeName = TEXT("Boss Pattern Check");
}

bool UBTDecorator_BossPatternCheck::CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto Boss = Cast<AEnemy_BossBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Boss) return false;

    OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), Boss->GetPatternNum());

	return true;
}