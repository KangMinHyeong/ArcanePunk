
#include "Components/Character/APSkillBaseQ.h"

#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APSkillNumber.h"

UAPSkillBaseQ::UAPSkillBaseQ()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAPSkillBaseQ::BeginPlay()
{
	Super::BeginPlay();
}

void UAPSkillBaseQ::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAPSkillBaseQ::SkillBase_Q(uint8 Second)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	switch(Second)
	{
		case 1:
		Skill_QQ();
		break;
		
		case 2:
		Skill_QE();
		break;

		case 3:
		Skill_QSpace();
		
		break;

		case 4:
		Skill_QShift();
		break;
	}
}

void UAPSkillBaseQ::Skill_QQ()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->GetAPSkillNumberComponent()->BindSkill(OwnerCharacter->GetQSkill(),1);
}

void UAPSkillBaseQ::Skill_QE()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->GetAPSkillNumberComponent()->BindSkill(OwnerCharacter->GetQSkill(),2);
}

void UAPSkillBaseQ::Skill_QSpace()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->GetAPSkillNumberComponent()->BindSkill(OwnerCharacter->GetQSkill(),3);
}

void UAPSkillBaseQ::Skill_QShift()
{

}
