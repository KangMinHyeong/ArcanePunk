#include "Components/Character/APSkillBaseSpace.h"

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
		// Skill_SpaceQ();
		break;
		
		case 2:
		// Skill_SpaceE();
		break;

		case 3:
		// Skill_SpaceShift();
		break;

		case 4:
		// Skill_SpaceSpace();
		break;
	}
}
