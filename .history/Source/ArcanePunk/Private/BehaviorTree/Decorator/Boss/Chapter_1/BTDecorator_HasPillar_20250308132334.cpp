
#include "BehaviorTree/Decorator/Boss/Chapter_1/BTDecorator_HasPillar.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Character/ArcanePunkCharacter.h"

UBTDecorator_HasPillar::UBTDecorator_HasPillar()
{
    NodeName = TEXT("Has Pillar");
}

bool UBTDecorator_HasPillar::CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    // auto Boss = Cast<AEnemy_Boss>(OwnerComp.GetAIOwner()->GetPawn());
	// if (!Boss) return false;

    // if(!Boss->HasPillar()) return false;
	return true;
}
