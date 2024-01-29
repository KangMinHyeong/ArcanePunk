
#include "Components/Character/APSkillBaseE.h"

#include "Components/Character/APSkillHubComponent.h"

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

void UAPSkillBaseE::SkillBase_E(ESkillKey Second)
{
	switch(Second)
	{
		case ESkillKey::Q:
		Skill_EQ();
		break;
		
		case ESkillKey::E:
		Skill_EE();
		break;

		case ESkillKey::R:
		Skill_ESpace();		
		break;

	}
}
void UAPSkillBaseE::Skill_EQ()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->BindSkill(OwnerCharacter->GetESkill(), ESkillTypeState::Type_Q, ESkillKey::E);
}

void UAPSkillBaseE::Skill_EE()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->BindSkill(OwnerCharacter->GetESkill(), ESkillTypeState::Type_E, ESkillKey::E);
}

void UAPSkillBaseE::Skill_ESpace()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->BindSkill(OwnerCharacter->GetESkill(), ESkillTypeState::Type_R, ESkillKey::E);
}

void UAPSkillBaseE::Skill_EShift()
{

}
