
#include "Components/Character/APSkillBaseShift.h"

UAPSkillBaseShift::UAPSkillBaseShift()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAPSkillBaseShift::BeginPlay()
{
	Super::BeginPlay();
}

void UAPSkillBaseShift::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAPSkillBaseShift::SkillBase_Shift(uint8 Second)
{
	switch(Second)
	{
		case 1:
		//Skill_ShiftQ();
		break;
		
		case 2:
		//Skill_ShiftE();
		break;

		case 3:
		//Skill_ShiftShift();
		break;

		case 4:
		//Skill_ShiftSpace();
		break;
	}
}
