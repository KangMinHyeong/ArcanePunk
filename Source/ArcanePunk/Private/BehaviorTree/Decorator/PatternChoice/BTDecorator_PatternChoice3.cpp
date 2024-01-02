
#include "BehaviorTree/Decorator/PatternChoice/BTDecorator_PatternChoice3.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_PatternChoice3::UBTDecorator_PatternChoice3()
{
    NodeName = TEXT("Pattern Choice 3");
}

bool UBTDecorator_PatternChoice3::CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    int32 PatternNum = OwnerComp.GetBlackboardComponent()->GetValueAsInt(GetSelectedBlackboardKey());

    if(PatternNum == 3) {bResult = true;}
    else {bResult = false;}

	return bResult;
}