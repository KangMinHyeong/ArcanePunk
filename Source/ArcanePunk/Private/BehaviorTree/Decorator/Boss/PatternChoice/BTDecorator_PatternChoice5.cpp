
#include "BehaviorTree/Decorator/Boss/PatternChoice/BTDecorator_PatternChoice5.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_PatternChoice5::UBTDecorator_PatternChoice5()
{
    NodeName = TEXT("Pattern Choice 5");
}

bool UBTDecorator_PatternChoice5::CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    int32 PatternNum = OwnerComp.GetBlackboardComponent()->GetValueAsInt(GetSelectedBlackboardKey());

    if(PatternNum == 5) {bResult = true;}
    else {bResult = false;}

	return bResult;
}