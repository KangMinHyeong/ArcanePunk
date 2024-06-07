
#include "Character/APCharacterBase.h"

#include "Components/Character/APMovementComponent.h"
#include "Components/Character/APAttackComponent.h"
#include "Components/Character/APHitPointComponent.h"
#include "Components/Character/APAnimHubComponent.h"
#include "NiagaraComponent.h"

AAPCharacterBase::AAPCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	CrowdControlComponent = CreateDefaultSubobject<UAPCrowdControlComponent>(TEXT("CrowdControlComponent"));
	MoveComponent = CreateDefaultSubobject<UAPMovementComponent>(TEXT("MoveComponent"));
	AttackComponent = CreateDefaultSubobject<UAPAttackComponent>(TEXT("AttackComponent"));
	HitPointComponent = CreateDefaultSubobject<UAPHitPointComponent>(TEXT("HitPointComponent"));
	AnimHubComponent = CreateDefaultSubobject<UAPAnimHubComponent>(TEXT("AnimHubComponent"));
	StunEffectComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("StunEffectComp"));

	StunEffectComp->SetupAttachment(GetMesh());
}

void AAPCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	CrowdControlComponent->BindCrowdComp();
	MoveComponent->SetBind();
}

bool AAPCharacterBase::IsDead()
{
	bool bIsDead = false;
	if(MyPlayerTotalStatus.PlayerDynamicData.HP<=KINDA_SMALL_NUMBER)
	{
		bIsDead = true;
	}
    return bIsDead;
}

void AAPCharacterBase::UpdateStatus()
{
}

void AAPCharacterBase::SetAttackRotation()
{
}

float AAPCharacterBase::CriticalCalculate()
{
	float Percent = FMath::RandRange(0.0f, 100.0f);
	if(Percent <= MyPlayerTotalStatus.PlayerDynamicData.CriticalPercent)
	{
		bCriticalAttack = true;
		return MyPlayerTotalStatus.PlayerDynamicData.CriticalStep * (MyPlayerTotalStatus.PlayerDynamicData.CriticalDamageCoefficient);
	}
	else {bCriticalAttack= false;}
    return 1.0f;
}
