
#include "BehaviorTree/Decorator/Boss/Chapter_1/BTDecorator_PillarThrowCheck.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Character/ArcanePunkCharacter.h"

UBTDecorator_PillarThrowCheck::UBTDecorator_PillarThrowCheck()
{
    NodeName = TEXT("Pillar Throw Check");
}

bool UBTDecorator_PillarThrowCheck::CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    // auto Boss = Cast<AEnemy_Boss>(OwnerComp.GetAIOwner()->GetPawn());
	// if (!Boss) return false;

    // auto Player = Cast<AArcanePunkCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
    // if (!Player) return false;

    // if(ThrowDist < (Boss->GetActorLocation() - Player->GetActorLocation()).Size()) return false;

	return true;
}
