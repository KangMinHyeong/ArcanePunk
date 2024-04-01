
#include "Skill/APSkillActorBase.h"

#include "Character/ArcanePunkCharacter.h"
#include "Components/Character/APSkillHubComponent.h"

AAPSkillActorBase::AAPSkillActorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// SkillTypeComp = CreateDefaultSubobject<UAPSkillType>(TEXT("SkillTypeComp"));
	// SkillAbilityComp = CreateDefaultSubobject<UAPSkillAbility>(TEXT("SkillAbilityComp"));
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

void AAPSkillActorBase::SetSkill(FSkillAbilityNestingData SkillAbilityNestingData)
{
	// SkillAbilityComp->SetSkillAbility(SkillAbility, SkillCategory);

    // SkillTypeComp->SetSkillType(SkillType, SkillCategory);

	SkillAbilityData = SkillAbilityNestingData;
}

void AAPSkillActorBase::DeActivate(ESkillNumber SkillNumber)
{
	auto Character = Cast<AArcanePunkCharacter>(GetOwner()); if(!Character) return;
	UE_LOG(LogTemp, Display, TEXT("Your message"));
	TWeakObjectPtr<USkillNumberBase> SkillNum = Character->GetAPSkillHubComponent()->GetSKillNumberComponent(SkillNumber);
	if(SkillNum.IsValid())
	{
		SkillNum->bActivate = false;
	} 
}
