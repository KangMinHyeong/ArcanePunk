
#include "Skill/ArcaneCutter.h"

#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Character/ArcanePunkCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Enemy/Enemy_CharacterBase.h"

AArcaneCutter::AArcaneCutter()
{
	CutterTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("CutterTrigger"));
	CutterEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("CutterEffect"));

	SetRootComponent(CutterTrigger);
	CutterEffect->SetupAttachment(CutterTrigger);

    CutterMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("CutterMovementComponent"));
	CutterMovementComponent->MaxSpeed = CutterSpeed;
	CutterMovementComponent->InitialSpeed = CutterSpeed;

    SkillCategory = ESkillCategory::Projecitle;
}

void AArcaneCutter::BeginPlay()
{
    Super::BeginPlay();
}

void AArcaneCutter::Tick(float DeltaTime)
{
}

void AArcaneCutter::OnHitting(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	auto MyOwner = GetOwner();
	if(MyOwner == nullptr)
	{
		Destroy();
		return;
	} 
	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();

	OwnerCharacter = Cast<AArcanePunkCharacter>(MyOwner);
	if (OtherActor && OtherActor != this && OtherActor != MyOwner && OwnerCharacter.IsValid())
	{
		if(bStun) HitPointComp->SetCrowdControl(OtherActor, ECharacterState::Stun, StateTime);
		HitPointComp->DistinctHitPoint(Hit.Location, OtherActor);
		UGameplayStatics::ApplyDamage(OtherActor, OwnerCharacter->GetFinalATK() * DamageCoefficient, MyOwnerInstigator, this, DamageTypeClass);
        // HitImpact & DestroyEffect
	}
    Destroy();
}

void AArcaneCutter::SetSkill(ESkillTypeState SkillType, TArray<ESkillAbility> SkillAbility)
{
	Super::SetSkill(SkillType, SkillAbility);

	CutterEffect->SetNiagaraVariableLinearColor(TEXT("Color"),  EffectColor);
	BintHit();
}

float AArcaneCutter::GetCutterSpeed() const
{
    return CutterMovementComponent->InitialSpeed;
}

void AArcaneCutter::SetDeadTime(float DeadTime)
{
    GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AArcaneCutter::DestroySKill, DeadTime, false);
}

float AArcaneCutter::GetTriggerWide() const
{
    return CutterTrigger->GetScaledBoxExtent().X;
}

void AArcaneCutter::DestroySKill()
{
	Super::DestroySKill();
}

void AArcaneCutter::BintHit()
{
    // CutterTrigger->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    CutterTrigger->OnComponentHit.AddDynamic(this, &AArcaneCutter::OnHitting);
}
