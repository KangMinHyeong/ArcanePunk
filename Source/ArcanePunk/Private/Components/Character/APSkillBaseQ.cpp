
#include "Components/Character/APSkillBaseQ.h"

#include "Components/Character/APSkillHubComponent.h"

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

void UAPSkillBaseQ::SkillBase_Q(ESkillKey Second)
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	switch(Second)
	{
		case ESkillKey::Q:
		Skill_QQ();
		break;
		
		case ESkillKey::E:
		Skill_QE();
		break;

		case ESkillKey::R:
		Skill_QSpace();
		break;
	}
}

void UAPSkillBaseQ::Skill_QQ()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->BindSkill(OwnerCharacter->GetQSkill(),ESkillTypeState::Type_Q ,ESkillKey::Q);
}

void UAPSkillBaseQ::Skill_QE()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->BindSkill(OwnerCharacter->GetQSkill(), ESkillTypeState::Type_E, ESkillKey::Q);
}

void UAPSkillBaseQ::Skill_QSpace()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->BindSkill(OwnerCharacter->GetQSkill(), ESkillTypeState::Type_R, ESkillKey::Q);
}

void UAPSkillBaseQ::Skill_QShift()
{

}
