
#include "Components/Character/APSkillBaseE.h"

UAPSkillBaseE::UAPSkillBaseE()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UAPSkillBaseE::BeginPlay()
{
	Super::BeginPlay();
}

void UAPSkillBaseE::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAPSkillBaseE::SkillBase_E(uint8 Second)
{
	switch(Second)
	{
		case 1:
		//Skill_EQ();
		break;
		
		case 2:
		//Skill_EE();
		break;

		case 3:
		//Skill_EShift();
		break;

		case 4:
		//Skill_ESpace();
		break;
	}
}
