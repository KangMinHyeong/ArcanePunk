
#include "Character/SkillRange/APSkillRange.h"


AAPSkillRange::AAPSkillRange()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RangeDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("RangeDecal"));

	SetRootComponent(RootComp);
	RangeDecal->SetupAttachment(RootComp);
}

void AAPSkillRange::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorTickEnabled(false);
	DefaultDecal = RangeDecal->GetDecalMaterial();
	DefaultSize = GetActorScale3D().Z;
}

void AAPSkillRange::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAPSkillRange::SetSkill(ESkillTypeState SkillType, TArray<ESkillAbility> SkillAbility)
{
	if(SkillAbility.Contains(ESkillAbility::Gigant)) SetActorScale3D(GetActorScale3D() * 2.0f);
	// if(SkillAbility.Contains(ESkillAbility::Homing)) SetHoming();
	// if(SkillAbility.Contains(ESkillAbility::Stun)) SetStun();
}
