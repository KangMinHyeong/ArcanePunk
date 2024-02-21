
#include "Skill/APSkillActorBase.h"

AAPSkillActorBase::AAPSkillActorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SkillTypeComp = CreateDefaultSubobject<UAPSkillType>(TEXT("SkillTypeComp"));
	SkillAbilityComp = CreateDefaultSubobject<UAPSkillAbility>(TEXT("SkillAbilityComp"));
	HitPointComp = CreateDefaultSubobject<UAPHitPointComponent>(TEXT("HitPointComp"));
}

void AAPSkillActorBase::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorTickEnabled(false);
	DefaultSize = GetActorScale3D().Y;
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AAPSkillActorBase::DestroySKill, DestroyTime, false);
}

void AAPSkillActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAPSkillActorBase::DestroySKill()
{
	Destroy();
	GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}

void AAPSkillActorBase::SetSkill(ESkillTypeState SkillType, TArray<ESkillAbility> SkillAbility)
{
	SkillAbilityComp->SetSkillAbility(SkillAbility, SkillCategory);

    SkillTypeComp->SetSkillType(SkillType, SkillCategory);
}