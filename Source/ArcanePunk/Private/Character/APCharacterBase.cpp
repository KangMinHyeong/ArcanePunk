
#include "Character/APCharacterBase.h"

#include "Components/Character/APMovementComponent.h"
#include "Components/Character/APAttackComponent.h"
#include "Components/Character/APHitPointComponent.h"
#include "Components/Character/APAnimHubComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraComponent.h"

AAPCharacterBase::AAPCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	CrowdControlComp = CreateDefaultSubobject<UAPCrowdControlComponent>(TEXT("CrowdControlComp"));
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
	CrowdControlComp->BindCrowdComp();
	MoveComponent->SetBind();
	DefaultMaterial = GetMesh()->GetMaterials();
	DefaultSlip = GetCharacterMovement()->BrakingFrictionFactor;
}

void AAPCharacterBase::ResetDefaultMaterial()
{
	int32 Index = 0;
	for(auto Mat : DefaultMaterial)
	{GetMesh()->SetMaterial(Index, Mat); Index++;}
}

bool AAPCharacterBase::IsDead()
{
	bool bIsDead = false;
	if(MyPlayerTotalStatus.PlayerDynamicData.HP<=KINDA_SMALL_NUMBER)
	{
		bIsDead = true;
		CrowdControlComp->PlayStateEffect(ECharacterState::Frozen, false);
	}
    return bIsDead;
}

void AAPCharacterBase::UpdateStatus()
{
}

void AAPCharacterBase::SetAttackRotation()
{
}

float AAPCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if(bBlockMode) return 0.0f;

	bHitting = true;

	int32 Index = 0;
	for(auto Mat : DefaultMaterial)
	{GetMesh()->SetMaterial(Index, HitMaterial); Index++;}

	GetWorldTimerManager().SetTimer(HitTimerHandle, this, &AAPCharacterBase::OnHittingEnd, HitMotionTime, false);

    return DamageAmount;
}

void AAPCharacterBase::OnHittingEnd()
{
	bHitting = false;
	GetCharacterMovement()->BrakingFrictionFactor = DefaultSlip;
	ResetDefaultMaterial();

	GetWorldTimerManager().ClearTimer(HitTimerHandle);
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
