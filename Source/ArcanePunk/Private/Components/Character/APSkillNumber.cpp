#include "Components/Character/APSkillNumber.h"

#include "Components/Character/SkillNumber/SkillNumber1.h"
#include "Components/Character/SkillNumber/SkillNumber2.h"
#include "Components/Character/SkillNumber/SkillNumber3.h"

UAPSkillNumber::UAPSkillNumber()
{
	PrimaryComponentTick.bCanEverTick = false;

	Skill_1 = CreateDefaultSubobject<USkillNumber1>(TEXT("Skill_1"));
	Skill_2 = CreateDefaultSubobject<USkillNumber2>(TEXT("Skill_2"));
	Skill_3 = CreateDefaultSubobject<USkillNumber3>(TEXT("Skill_3"));
}

void UAPSkillNumber::BeginPlay()
{
	Super::BeginPlay();	
}

void UAPSkillNumber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAPSkillNumber::BindSkill(uint8 SkillNumber)
{
	switch(SkillNumber)
	{
		case 1:
		Skill_1->PlaySkill();
		break;

		case 2:
		Skill_2->PlaySkill();
		break;

		case 3:
		Skill_3->PlaySkill();
		break;

		case 4:
		
		break;
	}
}

USkillNumber1 *UAPSkillNumber::GetSkillNumber1()
{
    return Skill_1;
}

USkillNumber2 *UAPSkillNumber::GetSkillNumber2()
{
    return Skill_2;
}

USkillNumber3 *UAPSkillNumber::GetSkillNumber3()
{
    return Skill_3;
}
