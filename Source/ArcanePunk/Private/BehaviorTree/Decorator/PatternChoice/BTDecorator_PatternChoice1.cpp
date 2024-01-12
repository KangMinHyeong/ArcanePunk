
#include "BehaviorTree/Decorator/PatternChoice/BTDecorator_PatternChoice1.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_PatternChoice1::UBTDecorator_PatternChoice1()
{
    NodeName = TEXT("Pattern Choice 1");
}

bool UBTDecorator_PatternChoice1::CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    int32 PatternNum = OwnerComp.GetBlackboardComponent()->GetValueAsInt(GetSelectedBlackboardKey());

    if(PatternNum == 1) {bResult = true;}
    else {bResult = false;}

	return bResult;
}