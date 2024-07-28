
#include "BehaviorTree/Decorator/Boss/PatternChoice/BTDecorator_PatternChoice6.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_PatternChoice6::UBTDecorator_PatternChoice6()
{
    NodeName = TEXT("Pattern Choice 6");
}

bool UBTDecorator_PatternChoice6::CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    int32 PatternNum = OwnerComp.GetBlackboardComponent()->GetValueAsInt(GetSelectedBlackboardKey());

    if(PatternNum == 6) {bResult = true;}
    else {bResult = false;}

	return bResult;
}