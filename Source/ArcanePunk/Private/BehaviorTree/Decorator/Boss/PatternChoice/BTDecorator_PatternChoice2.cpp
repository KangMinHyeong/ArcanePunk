
#include "BehaviorTree/Decorator/Boss/PatternChoice/BTDecorator_PatternChoice2.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_PatternChoice2::UBTDecorator_PatternChoice2()
{
    NodeName = TEXT("Pattern Choice 2");
}

bool UBTDecorator_PatternChoice2::CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    int32 PatternNum = OwnerComp.GetBlackboardComponent()->GetValueAsInt(GetSelectedBlackboardKey());

    if(PatternNum == 2) {bResult = true;}
    else {bResult = false;}

	return bResult;
}