
#include "Character/SkillRange/APSkillRange_TwoCircle.h"

#include "PlayerController/ArcanePunkPlayerController.h"

AAPSkillRange_TwoCircle::AAPSkillRange_TwoCircle()
{
    RangeDecal_2 = CreateDefaultSubobject<UDecalComponent>(TEXT("RangeDecal_2"));

    RangeDecal_2->SetupAttachment(RangeDecal);
}

void AAPSkillRange_TwoCircle::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void AAPSkillRange_TwoCircle::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData)
{
    Super::SetSkill(SkillAbilityNestingData);


}
