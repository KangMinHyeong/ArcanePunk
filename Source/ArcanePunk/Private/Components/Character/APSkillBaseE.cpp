
#include "Components/Character/APSkillBaseE.h"

#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APSkillNumber.h"

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
		Skill_EQ();
		break;
		
		case 2:
		Skill_EE();
		break;

		case 3:
		Skill_ESpace();		
		break;

		case 4:
		//Skill_EShift();
		break;
	}
}
void UAPSkillBaseE::Skill_EQ()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->GetAPSkillNumberComponent()->BindSkill(OwnerCharacter->GetESkill(),1);
}

void UAPSkillBaseE::Skill_EE()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->GetAPSkillNumberComponent()->BindSkill(OwnerCharacter->GetESkill(),2);
}

void UAPSkillBaseE::Skill_ESpace()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->GetAPSkillNumberComponent()->BindSkill(OwnerCharacter->GetESkill(),3);
}

void UAPSkillBaseE::Skill_EShift()
{

}
