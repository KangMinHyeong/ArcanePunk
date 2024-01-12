#include "Components/Character/APSkillBaseSpace.h"

#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APSkillNumber.h"

UAPSkillBaseSpace::UAPSkillBaseSpace()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAPSkillBaseSpace::BeginPlay()
{
	Super::BeginPlay();
}

void UAPSkillBaseSpace::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAPSkillBaseSpace::SkillBase_Space(uint8 Second)
{
	switch(Second)
	{
		case 1:
		Skill_SpaceQ();
		break;
		
		case 2:
		Skill_SpaceE();
		break;

		case 3:
		Skill_SpaceSpace();
		break;

		case 4:
		// Skill_SpaceShift();
		break;
	}
}

void UAPSkillBaseSpace::Skill_SpaceQ()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->GetAPSkillNumberComponent()->BindSkill(OwnerCharacter->GetRSkill(),1);
}

void UAPSkillBaseSpace::Skill_SpaceE()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->GetAPSkillNumberComponent()->BindSkill(OwnerCharacter->GetRSkill(),2);
}

void UAPSkillBaseSpace::Skill_SpaceSpace()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->GetAPSkillNumberComponent()->BindSkill(OwnerCharacter->GetRSkill(),3);
}

void UAPSkillBaseSpace::Skill_SpaceShift()
{
}