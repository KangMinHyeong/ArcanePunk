#include "Components/Character/APSkillBaseSpace.h"

#include "Components/Character/APSkillHubComponent.h"

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

void UAPSkillBaseSpace::SkillBase_Space(ESkillKey Second)
{
	switch(Second)
	{
		case ESkillKey::Q:
		Skill_SpaceQ();
		break;
		
		case ESkillKey::E:
		Skill_SpaceE();
		break;

		case ESkillKey::R:
		Skill_SpaceSpace();
		break;
	}
}

void UAPSkillBaseSpace::Skill_SpaceQ()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	// OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->BindUltSkill(OwnerCharacter->GetRSkill(),  ESkillTypeState::Type_Q, ESkillKey::R);
}

void UAPSkillBaseSpace::Skill_SpaceE()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	// OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->BindUltSkill(OwnerCharacter->GetRSkill(),  ESkillTypeState::Type_E, ESkillKey::R);
}

void UAPSkillBaseSpace::Skill_SpaceSpace()
{
	auto OwnerCharacter = Cast<AArcanePunkCharacter>(GetOwner());
	if(!OwnerCharacter) return;

	// OwnerCharacter->GetAPSkillHubComponent()->GetAPSkillNumberComponent()->BindUltSkill(OwnerCharacter->GetRSkill(),  ESkillTypeState::Type_R, ESkillKey::R);
}

void UAPSkillBaseSpace::Skill_SpaceShift()
{
}