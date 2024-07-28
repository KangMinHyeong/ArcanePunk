
#include "BehaviorTree/Decorator/Boss/PatternChoice/BTDecorator_PatternChoice8.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_PatternChoice8::UBTDecorator_PatternChoice8()
{
    NodeName = TEXT("Pattern Choice 8");
}

bool UBTDecorator_PatternChoice8::CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    int32 PatternNum = OwnerComp.GetBlackboardComponent()->GetValueAsInt(GetSelectedBlackboardKey());

    if(PatternNum == 8) {bResult = true;}
    else {bResult = false;}

	return bResult;
}