
#include "BehaviorTree/Decorator/Boss/PatternChoice/BTDecorator_PatternChoice7.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_PatternChoice7::UBTDecorator_PatternChoice7()
{
    NodeName = TEXT("Pattern Choice 7");
}

bool UBTDecorator_PatternChoice7::CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    int32 PatternNum = OwnerComp.GetBlackboardComponent()->GetValueAsInt(GetSelectedBlackboardKey());

    if(PatternNum == 7) {bResult = true;}
    else {bResult = false;}

	return bResult;
}
