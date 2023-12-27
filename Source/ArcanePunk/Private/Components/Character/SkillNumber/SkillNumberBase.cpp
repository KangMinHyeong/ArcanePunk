#include "Components/Character/SkillNumber/SkillNumberBase.h"

USkillNumberBase::USkillNumberBase()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USkillNumberBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void USkillNumberBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USkillNumberBase::PlaySkill()
{
}
