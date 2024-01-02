
#include "BehaviorTree/Decorator/PatternChoice/BTDecorator_PatternChoice4.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_PatternChoice4::UBTDecorator_PatternChoice4()
{
    NodeName = TEXT("Pattern Choice 4");
}

bool UBTDecorator_PatternChoice4::CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    int32 PatternNum = OwnerComp.GetBlackboardComponent()->GetValueAsInt(GetSelectedBlackboardKey());

    if(PatternNum == 4) {bResult = true;}
    else {bResult = false;}

	return bResult;
}