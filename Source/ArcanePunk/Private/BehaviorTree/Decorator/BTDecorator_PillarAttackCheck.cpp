
#include "BehaviorTree/Decorator/BTDecorator_PillarAttackCheck.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Enemy/SkillActor/APPillarAttack.h"
#include "Enemy/Enemy_Boss.h"

UBTDecorator_PillarAttackCheck::UBTDecorator_PillarAttackCheck()
{
    NodeName = TEXT("Pillar Attack Check");
}

bool UBTDecorator_PillarAttackCheck::CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    auto Boss = Cast<AEnemy_Boss>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Boss) return false;

    TArray<AAPPillarAttack*> Pillars;
    for(auto Pillar : TActorRange<AAPPillarAttack>(GetWorld()))
    {
        if(!Pillar->IsHidden() && !Pillar->IsThrown()) Pillars.Emplace(Pillar);
    }

    if(Pillars.IsEmpty()) {return false;}
    else
    {
        if(Pillars.Num() >= 2)
        {
            auto AIOwner = OwnerComp.GetAIOwner()->GetPawn();
            Pillars.Sort([Boss](AActor& A, AActor& B) 
            {
                return (A.GetActorLocation() - Boss->GetActorLocation()).Size() > (B.GetActorLocation() - Boss->GetActorLocation()).Size();
            });
        }

        Boss->SetPillarAttack(true);
        OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), Pillars.Top());
    }

	return true;
}